#pragma once

#include "Standard.hpp"
#include "Function.hpp"

#define DIR_SEP "/"

#define DIR_ASSETS "Assets" DIR_SEP
#define DIR_SHADERS DIR_ASSETS "Shaders" DIR_SEP
#define DIR_TEXTURES DIR_ASSETS "Textures" DIR_SEP
#define DIR_SHADER_HEADER DIR_SHADERS "Header"
#define DIR_MODELS DIR_ASSETS "Models" DIR_SEP
//#define DIR_FONTS DIR_ASSETS "Fonts" DIR_SEP

#define DIR_VERT_EXTENSION ".vs"
#define DIR_FRAG_EXTENSION ".fs"
#define DIR_MODEL_EXTENSION ".pmf"
#define DIR_TEXTURE_EXTENSION ".png"

/*
  dangerous yet convenient macros for creating assets.
  Simply put the initialization code between the CONSTRUCT_ASSET 
  and DELETE_ASSET and the deletion between the DELETE_ASSET and
  END_ASSET, and bam! you've got yourself an Asset!
*/

#define CONSTRUCT_ASSET(name) const static Asset name([]() {
#define DELETE_ASSET }, []() {
#define END_ASSET })

NAMESPACE {
  
  void LogCurrentDirectory();

  struct Asset {
    Asset(function<void(void)> load_func,
	  function<void(void)> free_func);
    static Array<function<void(void)>> loaders;
    static Array<function<void(void)>> freers;
    static void loadAll();
    static void freeAll();
  };
}
