/* This encoder is for the image processing assignemnt part C
*  the task is to encrypt a png message into a noisy img so that the message cannot
*  detected or seen
*
*  to use this just build and go to where the exe file is
*		this would be in the Part_C_encoder\x64\debug folder
*  just give it the exe in the command prompt and give it a message.png file to encode into noise
*  the program will ask for a password, this is so that it is able to make the same noise each time.
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>

unsigned long hash_djb2(const char* str);
cv::Mat  Gausian_noise(cv::Mat src, double sigma, cv::RNG& rng);
void encode(cv::Mat &to_encrypt, cv::Mat& noise);

int main(int argc, char** argv) {
	//if the right number of argumments is not given terminates program
	if (argc != 2) {
		std::cout << "not enought arguments" << std::endl;
		return -1;
	}

	//gets password
	std::string password;
	std::cout << "Please input a password: ";
	std::getline(std::cin, password);
	
	//gets the hash vvalue
	unsigned long hash_Kernel = hash_djb2(password.c_str());

	// gets the messae img to put in nosie
	cv::Mat to_encrypt = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if (to_encrypt.empty()) {
		//if not there terminate program and display message
		std::cout << "there was no immage to encrypt" << std::endl;
		return -1;
	}

	//sets the rng class with speciffic seed given from password
	cv::RNG rng(hash_Kernel);
	//makes noise based on dimentions of to_encrypt
	cv::Mat gausian_noise_img = Gausian_noise(to_encrypt, 10, rng);

	//encodes to_encrypt into noise
	encode(to_encrypt, gausian_noise_img);
	
	//writes to file 
	cv::imwrite("../outputs/Part_C_encoded.png", gausian_noise_img);
	return 0;

}

unsigned long hash_djb2(const char* str)
{
	/*
	this function isnt mine, got it from here http://www.cse.yorku.ca/~oz/hash.html 
	*/
	unsigned long hash_function = 1;
	int cnt;

	while (cnt = *str++)
		hash_function = ((hash_function << 5) + hash_function) + cnt;

	return hash_function;
}


cv::Mat  Gausian_noise(cv::Mat src, double sigma, cv::RNG& rng){
	cv::Mat dst(src.rows, src.cols, CV_8UC1, cv::Scalar(255)); //sets blank img same size and src

	//goes through each pixel and gives it a gausian noise.
	for (int x = 0; x < dst.rows; x++) {
		for (int y = 0; y < dst.cols; y++) {
			dst.at<uchar>(x, y) = rng.gaussian(sigma);
		}
	}
	
	cv::imwrite("../Visual/Gausian.png",dst);
	//returns the noisey img
	return dst;
}

void encode(cv::Mat &to_encrypt, cv::Mat& noise) {
	//invert it so message is now white
	//scale it down so when adding to noise it only changes by 1 insead of 255
	to_encrypt = (255 - to_encrypt)/255; 

	//go through each pixel and add to noise
	//	if at 255 dont do anything
	for (int x = 0; x < noise.rows; x++) {
		for (int y = 0; y < noise.rows; y++) {
			if (noise.at<uchar>(x, y) < 255) {
				noise.at<uchar>(x, y) = noise.at<uchar>(x, y) + to_encrypt.at<uchar>(x, y);
			}
		}
	}
}