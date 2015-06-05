#!/usr/bin/python3

import struct
import bmesh

class PMFFile:

    PMF_TYPE_STATIC_NO_TEXTURE = 0
    PMF_TYPE_STATIC_TEXTURE = 1

    def __init__(self, filename):
        self.file = open(filename, 'bw')
        self.file.write(b'PMF') #signature
        self.file.write(b'\x01') #version

    def writeStruct(self, fmt, *arg):
        self.file.write(struct.pack("<"+fmt, *arg))

    def writeString(self, string):
        self.file.write(str.encode(string) + b'\0')

    def writeVec3f(self, vec):
        self.writeStruct("fff", vec.x, vec.y, vec.z)
        
    def loadMeshes(self, meshes):

        bm = bmesh.new()
        
        #header stuff
        self.writeStruct("I", len(meshes))

        #the body
        for mesh in meshes:

            bm.from_mesh(mesh)
            bmesh.ops.triangulate(bm, faces = bm.faces)
            
            self.writeString(mesh.name)
            self.writeStruct("B", PMF_TYPE_STATIC_NO_TEXTURE)
            self.writeStruct("L", len(bm.faces))

            for face in bm.faces:

                self.writeVec3f(face.normal)

                for vert in face.verts:

                    self.writeVec3f(vert)

    def flush(self):
        self.file.flush()

#if __name__ == "__main__":
#    pmf = PMFFile("Test.pmf")
#    pmf.flush()
