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
   int k = 0;
enum {L = 32};
   float temp = 0;
   float local_a[4096];
#pragma HLS ARRAY_PARTITION variable=local_a block factor=2 dim=1
   float local_b[4096];
#pragma HLS ARRAY_PARTITION variable=local_b block factor=2 dim=1
   float local_c[4096];
#pragma HLS ARRAY_PARTITION variable=local_c block factor=2 dim=1

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

for (j=0;j<num_elems;j++)
{
#pragma HLS loop_tripcount min=0 max=32
#pragma HLS UNROLL factor = 128
#pragma HLS PIPELINE
local_c[j] = local_a[j] * local_b[j];
}

for (i=0; i<num_elems; i++)
{
#pragma HLS loop_tripcount min=0 max=32
#pragma HLS UNROLL factor = 128
#pragma HLS PIPELINE

temp += local_c[i];
}
          
       

/*
#pragma HLS INLINE OFF

for(i=0;i<num_elems;i++)
{
#pragma HLS loop_tripcount min=0 max=4096
#pragma HLS PIPELINE II = 1
#pragma HLS dependence variable=temp inter false

       temp +=local_c[i];
}
*/
/*
for (i=0;i<num_elems;i++)
{
#pragma HLS loop_tripcount min=0 max=4096
#pragma HLS UNROLL
#pragma HLS PIPELINE II=1
local_c[i] = local_a[i] * local_b[i];
temp +=local_c[i];

}

for(j=0;j<num_elems;j++)
{
#pragma HLS loop_tripcount min=0 max=4096
#pragma HLS UNROLL
temp +=local_c[j];

}
*/

   *c = temp;

}

}  // extern "C"
