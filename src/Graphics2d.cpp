#include "Graphics2d.hpp"

NAMESPACE {

  const static u32 INITIAL_2D_DATA_SIZE = 100000;
  const static u32 2D_DATA_RESIZE_DELTA = 500;

  void Graphics2d::addRenderable(Renderable2d* rend) {
    renderables.push_back(rend);
  }

  void Graphics2d::render(GLFWwindow* win) {
    
    Renderable2dData* data = malloc(INITIAL_2D_DATA_SIZE,
				    *sizeof(Renderable2dData));
    fatalAssert(data != NULL,
		"Problem allocating memory for 2d data");
    u32 prev_data_size = 0;
    u32 data_size = 0;
    u32 allocated_data_size = INITIAL_2D_DATA_SIZE;
    
    for (Renderable2d* rend : renderables) {
      prev_data_size = data_size;
      data_size += rend->data.size();
      if (data_size > allocated_data_size) {
	u32 size_delta = data_size - allocated_data_size;
        allocated_data_size += size_delta > 2D_DATA_RESIZE_DELTA ?
	  size_delta : 2D_DATA_RESIZE_DELTA;
	data = realloc(data, allocated_data_size);
      }
      memcpy(data+prev_data_size, &rend->data[0], rend->data.size());
    }
    
    free(data);
  }

}
