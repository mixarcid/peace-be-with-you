import bpy
import bmesh
import struct
from bpy_extras.io_utils import ExportHelper

bl_info = {
    "name":         "PMF Exporter",
    "author":       "Michael Arcidiacono",
    "blender":      (2,6,2),
    "version":      (0,0,2),
    "location":     "File > Import-Export",
    "description":  "Export Peace Model Format (PMF) files",
    "category":     "Import-Export"
}

class PMFFile:

    TYPE_STATIC_NO_TEXTURE = 0
    TYPE_STATIC_TEXTURE = 1

    def __init__(self, filename):
        self.file = open(filename, 'bw')
        self.file.write(b'PMF') #signature
        self.file.write(b'\x02') #version

    def writeStruct(self, fmt, *arg):
        self.file.write(struct.pack("<"+fmt, *arg))

    def writeString(self, string):
        self.file.write(str.encode(string) + b'\0')

    def writeVec3f(self, vec):
        self.writeStruct("fff", vec.x, vec.y, vec.z)

    def writeVec2f(self, vec):
        self.writeStruct("ff", vec.x, vec.y)
        
    def loadMeshes(self, meshes):

        bm = bmesh.new()
        
        #header stuff
        self.writeStruct("I", len(meshes))

        #the body
        for mesh in meshes:

            bm.from_mesh(mesh)
            bmesh.ops.triangulate(bm, faces = bm.faces)

            uv_layer = bm.loops.layers.uv.active
            if uv_layer is None:
                raise Exception("No UV Layer!")
            
            self.writeString(mesh.name)
            self.writeStruct("B", PMFFile.TYPE_STATIC_TEXTURE)
            self.writeString(mesh.uv_textures.active.data[0].image.name)

            vert_uvs = [None]*(len(bm.verts))
            for face in bm.faces:
                for loop, vert in zip(face.loops, face.verts):
                    vert_uvs[vert.index] = loop[uv_layer].uv
            
            self.writeStruct("L", len(bm.verts))
            print(len(bm.verts))
            for vert in bm.verts:
                self.writeVec3f(vert.co)
                self.writeVec3f(vert.normal)
                self.writeVec2f(vert_uvs[vert.index])

            self.writeStruct("L", len(bm.faces))
            for face in bm.faces:
                for vert in face.verts:
                    self.writeStruct("L", vert.index)

    def flush(self):
        self.file.flush()

class PMFExport(bpy.types.Operator, ExportHelper):
    
    bl_idname = "export_pmf.pmf"
    bl_label = "PMF Exporter"
    bl_options = {'PRESET'}
    
    filename_ext = ".pmf"
                    
    def execute(self, context):

        if self.filepath == "":
            print("Please provide a valid filename")
            return {'FINISHED'}
        
        pmf = PMFFile(self.filepath)
        pmf.loadMeshes(bpy.data.meshes)
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
