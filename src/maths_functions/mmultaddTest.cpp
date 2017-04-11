//#include "sds_lib.h"
//#include <iostream>
//
//
//#include "mmultaddTest.hpp"
//#include "mmultadd.hpp"
//
//void perf_counter::init_arrays(float *A,  float *B, float *C, float *D, float *D_sw)
//{
//     for (int i = 0; i < N; i++) {
//          for (int j = 0; j < N; j++) {
//               A[i * N + j] = 1+i*N+j;
//               B[i * N + j] = rand() % (N * N);
//               C[i * N + j] = i;
//               D[i * N + j] = 0.0;
//               D_sw[i * N + j] = 0.0;
//          }
//     }
//}
//
//
//int perf_counter::result_check(float *D, float *D_sw)
//{
//     for (int i = 0; i < N * N; i++) {
//          if (D_sw[i] != D[i]) {
//               std::cout << "Mismatch: data index=" << i << "d=" << D_sw[i]
//                        << ", dout=" << D[i] << std::endl;
//               return 1;
//          }
//     }
//     return 0;
//}
//
//int mmult_test(float *A,  float *B, float *C, float *D, float *D_sw)
//{
//     std::cout << "Testing " << NUM_TESTS << " iterations of " << N << "x" << N
//               << " floating point mmultadd..." << std::endl;
//
//     perf_counter hw_ctr, sw_ctr;
//
//     for (int i = 0; i < NUM_TESTS; i++)
//     {
//          perf_counter::init_arrays(A, B, C, D, D_sw);
//
//          float tmp[N*N], tmp1[N*N];
//          sw_ctr.start();
//          mmult_golden(A, B, tmp);
//          madd_golden(tmp, C, D_sw);
//          sw_ctr.stop();
//
//          hw_ctr.start();
//          mmult(A, B, tmp1);
//          madd(tmp1, C, D);
//          hw_ctr.stop();
//
//          if (perf_counter::result_check(D, D_sw))
//               return 1;
//     }
//
//     uint64_t sw_cycles = sw_ctr.avg_cpu_cycles();
//     uint64_t hw_cycles = hw_ctr.avg_cpu_cycles();
//     double speedup = (double) sw_cycles / (double) hw_cycles;
//
//     std::cout << "Average number of CPU cycles running mmultadd in software: "
//               << sw_cycles << std::endl;
//     std::cout << "Average number of CPU cycles running mmultadd in hardware: "
//               << hw_cycles << std::endl;
//     std::cout << "Speed up: " << speedup << std::endl;
//
//     return 0;
//}
//
//
///*
// * MAIN TO RUN THIS TEST
// *
// * int test_passed = 0;
//     float *A, *B, *C, *D, *D_sw;
//
//     A = (float *)sds_alloc(N * N * sizeof(float));
//     B = (float *)sds_alloc(N * N * sizeof(float));
//     C = (float *)sds_alloc(N * N * sizeof(float));
//     D = (float *)sds_alloc(N * N * sizeof(float));
//     D_sw = (float *)malloc(N * N * sizeof(float));
//
//     if (!A || !B || !C || !D || !D_sw) {
//          if (A) sds_free(A);
//          if (B) sds_free(B);
//          if (C) sds_free(C);
//          if (D) sds_free(D);
//          if (D_sw) free(D_sw);
//          return 2;
//     }
//
//     test_passed = mmult_test(A, B, C, D, D_sw);
//
//     std::cout << "TEST " << (test_passed ? "FAILED" : "PASSED") << std::endl;
//
//     sds_free(A);
//     sds_free(B);
//     sds_free(C);
//     sds_free(D);
//     free(D_sw);
//
//     return (test_passed ? -1 : 0);
// */
