#include <stdio.h>

#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <iostream>
#include "opencv2/core/core.hpp"
//#include "opencv2/features2d/features2d.hpp"
//#include "opencv2/nonfree/features2d.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/nonfree/nonfree.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/opencv.hpp"
//#include "opencv2/imgproc/types_c.h"
//#include "opencv2/highgui.hpp"
//#include "opencv2/highgui/highgui_c.h"
#include <sstream>


//Drivers for our core
#include "xcombiner_top.h"
#include "xlloyds_kernel_top.h"

#define WEIGHTS_LENGTH 1296
#define INPUT_LENGTH 1024
#define OUPUT_LENGTH 784

#define WEIGHTS_ADDR 0x1FC00000
#define INPUT_FRAME_ADDR 0x1FC40000
//#define OUTPUT_FRAME_ADDR 0x1FCC0C00
#define OUTPUT_FRAME_ADDR 0x1FC50000

#define LLOYDS_KERNEL_ADDR_1 0x43C10000
//#define LLOYDS_KERNEL_ADDR_2 0x43C20000

//#define COMBINER_ADDR_1 0x43C00000
//#define COMBINER_ADDR_2 0x43C30000

//#define WEIGHTS_LENGTH 1296
//#define INPUT_LENGTH 1024
//#define OUPUT_LENGTH 784

#define IMG_SIZE 128 
#define K 8 
#define D 4 
#define L 10 // max number of clustering iterations

#define MAP_SIZE 40960000UL
#define MAP_MASK (MAP_SIZE - 1)

void *setup_reserved_mem(uint input_address);
void flush_caches();


int main(int argc, char* argv[] )
{
/*
	if(argc != 2)
	{
		printf("usage %s <linear address>\n", argv[0]);
		return -1;
	}

	unsigned linAddr = atoi(argv[1]);
*/

//	//Define the Hardware output container
//	cv::Mat default_output(cv::Size(IMG_SIZE,IMG_SIZE),CV_32SC3, cv::Scalar(60,60,60));
//	cv::Mat hw_outputFrame(cv::Size(IMG_SIZE,IMG_SIZE),CV_32SC3); //Setup the output image contained and give it a size
//	hw_outputFrame.data = (uchar *) setup_reserved_mem(KERNEL_INTERMEDIATE_ADDR);	//Point the container to the reserved RAM
//	default_output.copyTo(hw_outputFrame);


	int *weights_pointer = (int *) setup_reserved_mem(WEIGHTS_ADDR); //get a virtual address for the cluster centres for initialisation.
//	int *kernel_info_pointer = (int *) setup_reserved_mem(KERNEL_INTERMEDIATE_ADDR); //get a virtual address for the cluster centres for initialisation.
        int *input_frame_pointer = (int *) setup_reserved_mem(INPUT_FRAME_ADDR);
	//Input container
//	cv::Mat inFrame; //Temporary input container
//	inFrame = cv::imread("test_image.jpg"); //Read the input

	int *output_frame_pointer = (int *) setup_reserved_mem(OUTPUT_FRAME_ADDR);

	//Attempting some resizing
//	cv::Size size(IMG_SIZE,IMG_SIZE);
//	cv::resize(inFrame, inFrame, size); //Changing the image to size so that it complies with the HW modules
	//Convert the inFrame to the format that is recognised by the hardware
//	inFrame.convertTo(inFrame, CV_32SC3);
//	cv::Mat hw_inputFrame = inFrame.clone();
//	hw_inputFrame.data =(uchar *) setup_reserved_mem(INPUT_FRAME_ADDR); //Point the input container to the reserved RAM
//	inFrame.copyTo(hw_inputFrame);	

//	cv::Mat testOut;
//	testOut = hw_inputFrame; //Grab the hardware input frame to attempt to output
    	

//	testOut.convertTo(testOut, CV_8UC3); //convert into a sensible format that can be displayed
	//imshow("INPUT", testOut); //Print out the input that the HW sees

	// sample the initial clustering centers
//	int c_counter = 0;
//  	for (uint i=0; i<K; i++) {
//     		uint idx = rand() % IMG_SIZE;
//		
//     		for (uint d=0; d<D; d++) {
//     			int coord = testOut.at<cv::Vec3b>(idx,idx)[d];
//			*(weights_pointer+c_counter) = coord;
//			c_counter++;			
//     		}
//    	}

	//HARDWARE SETUP-----------------------------------------------------------------------
	//sets up the two IP cores, this needs to be turned into a function
	//Setup the kernel core parameters
	XLloyds_kernel_top kernel_dev_1 = setup_XLloyds_kernel_top(LLOYDS_KERNEL_ADDR_1);
	XLloyds_kernel_top_SetBlock_address(&kernel_dev_1, 0);
	XLloyds_kernel_top_SetData_points_addr(&kernel_dev_1, INPUT_FRAME_ADDR);
	XLloyds_kernel_top_SetCentres_in_addr(&kernel_dev_1, WEIGHTS_ADDR);
	XLloyds_kernel_top_SetOutput_addr(&kernel_dev_1, OUTPUT_FRAME_ADDR);
	XLloyds_kernel_top_SetN(&kernel_dev_1, 14);
	XLloyds_kernel_top_SetK(&kernel_dev_1, 14);
/*	XLloyds_kernel_top_SetInputDepth(&kernel_dev_1, 3);
	XLloyds_kernel_top_SetOutRowStart(&kernel_dev_1, 0);
	XLloyds_kernel_top_SetOutRowEnd(&kernel_dev_1, 14);
	XLloyds_kernel_top_SetOutColStart(&kernel_dev_1, 0);
	XLloyds_kernel_top_SetOutColEnd(&kernel_dev_1, 14);
	XLloyds_kernel_top_SetOutDepthStart(&kernel_dev_1, 0);
	XLloyds_kernel_top_SetOutDepthEnd(&kernel_dev_1, 4);
	XLloyds_kernel_top_SetInDepthStart(&kernel_dev_1, 0);
	XLloyds_kernel_top_SetInDepthEnd(&kernel_dev_1, 4);
*/

	printf("%i\n", XLloyds_kernel_top_GetBlock_address(&kernel_dev_1));
	printf("%i\n", XLloyds_kernel_top_GetData_points_addr(&kernel_dev_1));
	printf("%i\n", XLloyds_kernel_top_GetCentres_in_addr(&kernel_dev_1));
	printf("%i\n", XLloyds_kernel_top_GetOutput_addr(&kernel_dev_1));
	printf("%i\n", XLloyds_kernel_top_GetN(&kernel_dev_1));
	printf("%i\n", XLloyds_kernel_top_GetK(&kernel_dev_1));
/*	printf("%i\n", XLloyds_kernel_top_GetInputDepth(&kernel_dev_1));
	printf("%i\n",         XLloyds_kernel_top_GetOutRowStart(&kernel_dev_1));
	printf("%i\n",         XLloyds_kernel_top_GetOutRowEnd(&kernel_dev_1));
	printf("%i\n",         XLloyds_kernel_top_GetOutColStart(&kernel_dev_1));
	printf("%i\n",         XLloyds_kernel_top_GetOutColEnd(&kernel_dev_1));
	printf("%i\n",         XLloyds_kernel_top_GetOutDepthStart(&kernel_dev_1));
	printf("%i\n", XLloyds_kernel_top_GetOutDepthEnd(&kernel_dev_1));
	printf("%i\n", XLloyds_kernel_top_GetInDepthStart(&kernel_dev_1));
        printf("%i\n", XLloyds_kernel_top_GetInDepthEnd(&kernel_dev_1));
*/

	// set weights and input values
	for(int index = 0; index < WEIGHTS_LENGTH; index ++)
        {
	        weights_pointer[index] = 1;	
        }

	for(int index = 0; index < INPUT_LENGTH; index ++)
        {
                input_frame_pointer[index] = 1;	
        }

	//------------------------------------------------------------------------------------
	uint distortion = UINT_MAX;
	
	//starting kernel
//	XLloyds_kernel_top_SetBlock_address(&kernel_dev_1, block_address*sizeof(int)); //Reassign the kernel modules block address

	printf("starting kernel\n");
	XLloyds_kernel_top_Start(&kernel_dev_1); //Kick the Kernel block
	while(XLloyds_kernel_top_IsDone(&kernel_dev_1) != 1) {printf("kernel not yet complete\n");} //block for the first hardware stage
	printf("kernel complete\n");

	//Writing the golden output
        FILE *fp=fopen("test_out.txt", "w");
        char *line = NULL;
        size_t len = 0;
        fprintf(fp,"test text\n");
	u32 data = 1;
	fprintf(fp,"%i\n", data);
        fclose(fp);
   

	for(int index = 0; index < 11; index ++)
	{
		printf("%i ", output_frame_pointer[index]);
	}
	printf("\n");

//	for (uint clustering_iteration=0; clustering_iteration<L; clustering_iteration++) {

		//start the kernel
//		for(int block_address=0; block_address<IMG_SIZE*IMG_SIZE; block_address+=16)
//		{
//			XLloyds_kernel_top_SetBlock_address(&kernel_dev_1, block_address*sizeof(int)); //Reassign the kernel modules block address
//			XLloyds_kernel_top_Start(&kernel_dev_1); //Kick the Kernel block
//			while(XLloyds_kernel_top_IsDone(&kernel_dev_1) != 1) { } //block for the first hardware stage
//		}

//		XCombiner_top_Start(&combiner_dev_1); //now that the kernel block has finished, kick the combiner
//		while(XCombiner_top_IsDone(&combiner_dev_1) != 1) { } //block for the second hardware stage

		
//		uint new_distortion = XCombiner_top_GetDistortion_out(&combiner_dev_1);
	
		//One shot operation is now completed, attempting to print result
//		distortion = new_distortion;

	       // New clustering centers
//	        c_counter = 0;
//	        for (uint i=0; i<K; i++) {                
//	                for (uint d=0; d<D; d++) {                                                
//              	        c_counter++;
//	                }
//	        }
//	}


        //start the kernel for final output
//	XLloyds_kernel_top_SetUpdate_points(&kernel_dev_1, 1); // setting this to 1 will write a new image frame at KERNEL_INTERMEDIATE_ADDR

//        for(int block_address=0; block_address<IMG_SIZE*IMG_SIZE; block_address+=16)
//       {
//                XLloyds_kernel_top_SetBlock_address(&kernel_dev_1, block_address*sizeof(int)); //Reassign the kernel modules block address
//                XLloyds_kernel_top_Start(&kernel_dev_1); //Kick the Kernel block
//               while(XLloyds_kernel_top_IsDone(&kernel_dev_1) != 1) { } //block for the first hardware stage
//     	}

//	cv::Mat outFrame;
//	hw_outputFrame.copyTo(outFrame);
//	outFrame.convertTo(outFrame, CV_8UC3); //Convert into the right output format
	//imshow("OUTPUT", outFrame);

/*
	unsigned err_count = 0;
	
	//Testing against golden output
        FILE *fp=fopen("golden.mat", "r");
	char *line = NULL;
	size_t len = 0;
        for(int y=0;y<IMG_SIZE;y++)
        {
                for(int x=0;x<IMG_SIZE;x++)
                {
			getline(&line,&len,fp);
			int tmp_r = atoi(line);
			if(tmp_r != outFrame.at<cv::Vec3b>(y,x)[0])
				err_count++;

			getline(&line,&len,fp);
			int tmp_g = atoi(line);
			if(tmp_g != outFrame.at<cv::Vec3b>(y,x)[1])
				err_count++;
				
			getline(&line,&len,fp);
			int tmp_b = atoi(line);
			if(tmp_b != outFrame.at<cv::Vec3b>(y,x)[2])
				err_count++;

                }
        }
        fclose(fp);
*/
	//Writing the golden output
        //FILE *fp=fopen("golden.mat", "w");
	//char *line = NULL;
	//size_t len = 0;
        //for(int y=0;y<IMG_SIZE;y++)
        //{
        //        for(int x=0;x<IMG_SIZE;x++)
        //        {
	//		fprintf(fp,"%d\n%d\n%d\n", testOut.at<cv::Vec3b>(y,x)[0], testOut.at<cv::Vec3b>(y,x)[1], testOut.at<cv::Vec3b>(y,x)[2]);
        //        }
        //}
        //fclose(fp);

	
/*
	if((err_count - 47335) > 0) {
		std::stringstream filename;
		filename << "./outputs/" << argv[1] << ".jpg";
		imwrite(filename.str(), outFrame); //displaying the output frame
	}

	printf("%u,\t%u\n", linAddr, (err_count - 47335));
*/

//while(1)
//{
//	
//        if (cv::waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
//       {
//            break;
//       }
//}

	return 0;
}


void flush_caches()
{
        int cacheflush_fd = open("/dev/cacheflush", O_RDWR); //open the device file
	int junk = 1;
        write(cacheflush_fd, &junk, sizeof(unsigned int)); //&phys_addr_state
        close(cacheflush_fd);
}

void * setup_reserved_mem(uint input_address) //returns a pointer in userspace to the device
{
    void *mapped_base_reserved_mem;
    int memfd_reserved_mem;
    void *mapped_dev_base;
    off_t dev_base = input_address;

    memfd_reserved_mem = open("/dev/mem", O_RDWR | O_SYNC); //to open this the program needs to be run as root
        if (memfd_reserved_mem == -1) {
        printf("Can't open /dev/mem.\n");
        exit(0);
    }
//    printf("/dev/mem opened.\n"); 

    // Map one page of memory into user space such that the device is in that page, but it may not
    // be at the start of the page.

    mapped_base_reserved_mem = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd_reserved_mem, dev_base & ~MAP_MASK);
        if (mapped_base_reserved_mem == (void *) -1) {
        printf("Can't map the memory to user space.\n");
        exit(0);
    }
//    printf("Memory mapped at address %p.\n", mapped_base_reserved_mem); 

    // get the address of the device in user space which will be an offset from the base 
    // that was mapped as memory is mapped at the start of a page 

    mapped_dev_base = mapped_base_reserved_mem + (dev_base & MAP_MASK);
    return mapped_dev_base;
}


