#include <assert.h>

extern "C" {

void dot_prod_kernel(const float* a, const float* b, float* c, const int num_elems) {
#pragma HLS interface m_axi port = a offset = slave bundle = gmem
#pragma HLS interface m_axi port = b offset = slave bundle = gmem
#pragma HLS interface m_axi port = c offset = slave bundle = gmem
#pragma HLS interface s_axilite port = a bundle = control
#pragma HLS interface s_axilite port = b bundle = control
#pragma HLS interface s_axilite port = c bundle = control
#pragma HLS interface s_axilite port = num_elems bundle = control
#pragma HLS interface s_axilite port = return bundle = control
  assert(num_elems <= 4096);  // this helps HLS estimate the loop trip count
  /***************************
   * your code goes here ... *
   ***************************/
   int i = 0;
   int j = 0;
   float temp = 0;

   float local_a[4096];
#pragma HLS ARRAY_PARTITION variable=local_a block factor=512 dim=1
   float local_b[4096];
#pragma HLS ARRAY_PARTITION variable=local_b block factor=512 dim=1
   float local_c[4097];
#pragma HLS ARRAY_PARTITION variable=local_c block factor=512 dim=1
   local_c[0] = 0;
#pragma HLS INLINE OFF
for(i = 0; i<num_elems; i++)
{
#pragma HLS UNROLL 
#pragma HLS PIPELINE II=1
      local_a[i] = a[i];        
}

for(i = 0; i<num_elems; i++)
{
#pragma HLS UNROLL 
#pragma HLS PIPELINE II=1
      local_b[i] = b[i];
}

#pragma HLS INLINE OFF

for (j=0;j<num_elems;j=j+32)
{

#pragma HLS loop_tripcount min=0 max=128 
#pragma HLS UNROLL

for (i=j; i<j+32; i++)
{
#pragma HLS loop_tripcount min=0 max=32
#pragma HLS PIPELINE II=1
local_c[i+1] = local_c[i] + local_a[i] * local_b[i];
}

temp += local_c[i];
local_c[i] = 0;
}
/*
for(i=0;i<num_elems;i++)
{
//#pragma HLS loop_tripcount min=0 max=32
#pragma HLS UNROLL
#pragma HLS PIPELINE II=1
temp+=local_c[i];
}
*/
   *c = temp;

}

}  // extern "C"
