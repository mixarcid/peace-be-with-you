import bpy
import bmesh
import struct
from bpy_extras.io_utils import ExportHelper
from mathutils import Matrix, Vector
from functools import reduce

bl_info = {
    "name":         "PMF format",
    "author":       "Michael Arcidiacono",
    "blender":      (2,6,2),
    "version":      (0,0,2),
    "location":     "File > Import-Export",
    "description":  "Export Peace Model Format (PMF) files",
    "category":     "Import-Export"
}

VEC_OFFSET = 0.0001
def vec2Cmp(v1, v2):
    x = (v1.x < v2.x + VEC_OFFSET and v2.x < v1.x + VEC_OFFSET)
    y = (v1.y < v2.y + VEC_OFFSET and v2.y < v1.y + VEC_OFFSET)
    return x and y

class PMFFile:

    VERSION = b'\x04'

    TYPE_STATIC_NO_TEXTURE = 0
    TYPE_STATIC_TEXTURE = 1
    TYPE_BONED_TEXTURE = 2

    MAX_WEIGHTS_PER_VERTEX = 4

    def __init__(self, filename):
        self.file = open(filename, 'bw')
        self.file.write(b'PMF')
        self.file.write(PMFFile.VERSION)

    def writeStruct(self, fmt, *arg):
        self.file.write(struct.pack("<"+fmt, *arg))

    def writeString(self, string):
        self.file.write(str.encode(string) + b'\0')

    def writeVec3f(self, vec):
        self.writeStruct("fff", vec.x, vec.y, vec.z)

    def writeVec2f(self, vec):
        self.writeStruct("ff", vec.x, vec.y)

    def writeQuaternionf(self, q):
        self.writeStruct("ffff", q.w, q.x, q.y, q.z)
        
    def loadMeshes(self, mesh_objects, scene, actions):

        bm = bmesh.new()
        
        #header stuff
        self.writeStruct("I", len(mesh_objects))

        #the body
        for mesh_obj in mesh_objects:

            mesh = mesh_obj.data
            bm.from_mesh(mesh)
            bmesh.ops.triangulate(bm, faces = bm.faces)

            uv_layer = bm.loops.layers.uv.active
            if uv_layer is None:
                raise Exception("No UV Layer!")

            armature = mesh_obj.find_armature()
            if armature is None or armature.animation_data is None:
                mesh_type = PMFFile.TYPE_STATIC_TEXTURE
            else:
                mesh_type = PMFFile.TYPE_BONED_TEXTURE
                bone_names = [bone.name for bone in armature.data.bones]
            
            self.writeString(mesh.name)
            self.writeStruct("B", mesh_type)
            #self.writeStruct("I", flags)
            #self.writeString(mesh.uv_textures.active.data[0].image.name)

            vert_uvs = [[] for x in range(len(bm.verts))]
            vert_indexes = [0 for x in range(len(bm.verts))]
            vert_length = 0
            for face in bm.faces:
                for loop, vert in zip(face.loops, face.verts):
                    should_append = True
                    for uv in vert_uvs[vert.index]:
                        if vec2Cmp(uv, loop[uv_layer].uv):
                            should_append = False
                            #print("YEAH!")
                    if should_append:
                        vert_uvs[vert.index].append(loop[uv_layer].uv)
                        vert_length = vert_length + 1
                        vert_indexes = vert_indexes[0:vert.index+1] \
                                       + [i + 1 for i in vert_indexes[vert.index+1:]]
            
            self.writeStruct("L", vert_length)
            #print(len(bm.verts))
            for vert in mesh.vertices:
                #self.writeStruct"B", len(vert_uvs[vert.index])
                #print("UVs: " + str(vert_uvs[vert.index]))
                for uv in vert_uvs[vert.index]:
                    self.writeVec3f(vert.co)
                    self.writeVec3f(vert.normal)
                    self.writeVec2f(uv)
                    if mesh_type == PMFFile.TYPE_BONED_TEXTURE:
                        bone_indexes = []
                        bone_weights = []
                        for g in vert.groups:
                            for n in range(len(bone_names)):
                                if mesh_obj.vertex_groups[g.group].name == bone_names[n]:
                                    bone_indexes.append(n)
                                    bone_weights.append(g.weight)
                                    break
                        b_len = len(bone_indexes)
                        b_data = zip(bone_indexes, bone_weights)            
                        if (b_len > PMFFile.MAX_WEIGHTS_PER_VERTEX):
                            b_len = PMFFile.MAX_WEIGHTS_PER_VERTEX
                            b_data = list(sorted(b_data, key = lambda x: x[1]))[:PMFFile.MAX_WEIGHTS_PER_VERTEX]
                        self.writeStruct("B", b_len)
                        for d in b_data:
                            self.writeStruct("I", d[0])
                            self.writeStruct("f", d[1])

            self.writeStruct("L", len(bm.faces))
            for face in bm.faces:
                for vert, loop in zip(face.verts, face.loops):
                    n = 0
                    for uv in vert_uvs[vert.index]:
                        if vec2Cmp(uv, loop[uv_layer].uv):
                            break
                        n = n + 1
                    #print("My index: " + str(vert_indexes[vert.index]) + " | real index: " + str(vert.index))
                    self.writeStruct("L", vert_indexes[vert.index] + n)

            if mesh_type == PMFFile.TYPE_BONED_TEXTURE:
                        
                def getTransform(bone):
                    inv_rot = Matrix()
                    final_rot = bone.matrix_channel.decompose()[1]
                    final_trans = Vector((0,0,0))
                    final_mat = Matrix()
                    tot_bones = list(reversed(bone.parent_recursive)) + [bone]
                    for b in tot_bones:
                        head = armature.matrix_world * b.head - final_trans
                        rot = (b.matrix_channel.decompose()[1]).to_matrix()
                        rot.resize_4x4()
                        r = rot * inv_rot
                        inv_rot = inv_rot * r.inverted()
                        there = Matrix.Translation(head)
                        back = Matrix.Translation(-head)
                        mat = there * r * back
                        final_mat = final_mat * mat
                        trans = (mat).decompose()[0]
                        final_trans += trans
                    return final_rot, final_trans
                        
                    """if bone is None:
                        return Matrix()
                    head = armature.matrix_world * bone.head
                    r = (getLocalMat(bone).decompose()[1]).to_matrix()
                    r.resize_4x4()
                    head_mat = Matrix.Translation(head)
                    return head_mat * r * Matrix.inverted(head_mat)"""      
                        
                def writeBone(bone):
                    rot, trans = getTransform(bone)
                    self.writeVec3f(trans)
                    self.writeQuaternionf(rot)
                
                self.writeStruct("I", len(armature.data.bones))
                bpy.context.scene.frame_set(0)
                for bone in armature.pose.bones:
                    writeBone(bone)
                
                self.writeStruct("I", len(actions))
                for act in actions:
                    armature.animation_data.action = act

                    keyframes = [0]
                    bpy.ops.screen.frame_jump(0)
                    while bpy.ops.screen.keyframe_jump() == {'FINISHED'}:
                        keyframes.append(bpy.context.scene.frame_current-1)

                    self.writeString(act.name)
                    self.writeStruct("I", len(keyframes))
                    #print(keyframes)
                    for frame in keyframes:
                        bpy.context.scene.frame_set(frame)
                        self.writeStruct("f", frame/bpy.context.scene.render.fps)
                        for bone in armature.pose.bones:
                            #print(bone.rotation_quaternion)
                            writeBone(bone)
                            

    def flush(self):
        self.file.flush()

class PMFExport(bpy.types.Operator, ExportHelper):
    
    bl_idname = "export_pmf.pmf"
    bl_label = "Export .pmf"
    bl_options = {'PRESET'}
    
    filename_ext = ".pmf"
                    
    def execute(self, context):

        if self.filepath == "":
            raise Exception("Please provide a valid filename")
            return {'FINISHED'}
        
        pmf = PMFFile(self.filepath)
        meshes = [m for m in bpy.data.objects if m.type == 'MESH']
        pmf.loadMeshes(meshes, bpy.context.scene, bpy.data.actions)
        pmf.flush()
        return {'FINISHED'}

def menu_func(self, context):
        self.layout.operator(PMFExport.bl_idname, text="Peace Model Format (.pmf)")

def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)
    
def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
    register()
