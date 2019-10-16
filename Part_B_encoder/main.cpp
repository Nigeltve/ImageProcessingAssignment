/*	this is the encoder for the Image Processing Analysis assignment Part B
*	what this program does is take a message and scrable it based on a password
*	and encryptes it in a given carrier image
*	
*	to run
*		just run the exe with the carrier image then the message image as parametts
*	
*	the over method of scrableing is to flatten then swap randomly coordinates in the
*	flattend vector based on a pass word, then to swap the actual values in the image.
*
*	encryption works liek the following
*		invert -> scale down (0-1 instead of 0-255) -> add to carrier img
*
*	the hash_djb2 is not mine i got it from site stated in the function (site where the function is given by lectuere)
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

unsigned long hash_djb2(const char* str);
void encrypt(cv::Mat& carrier, cv::Mat& to_encrypt);
void flatten_shuffle(std::vector<cv::Point2i>& flat_coord, std::vector<int>& shuffle_order, cv::Mat& img, cv::RNG& rng);

int main(int argc, char** argv) {

	//set right number of aruments
	if (argc != 3) {
		std::cout << "not enough arguments." << std::endl;
		std::cout << "\tcarrier image: " << argv[1] << std::endl;
		std::cout << "\tto encrypt image: " << argv[2] << std::endl;

		return -1;
	}
	//param image one
	cv::Mat carrier = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if (carrier.empty()) {std::cout << "carrier image is empty" << std::endl; return -1;} //terminates if not there

	//to encrypt
	cv::Mat to_encrypt = cv::imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
	if (to_encrypt.empty()) { std::cout << "there is no image to encrypt" << std::endl; return-1;}//terminates if not there

	//sets password
	std::string password;
	std::cout << "what pass word do you want: ";
	std::getline(std::cin, password);

	//sets the seed value
	unsigned long hash_Number = hash_djb2(password.c_str());
	std::cout << hash_Number << std::endl;
	//sets the RNG class with seed
	cv::RNG rng(hash_Number);
	
	//display what message your goign to encrypt
	cv::namedWindow("preScramble");
	cv::imshow("preScramble", to_encrypt);
	
	//sets the vectors used for swapping
	std::vector<int> idx_swap;
	std::vector<cv::Point2i> flat_img;

	//method for actually flattening and swapping
	flatten_shuffle(flat_img, idx_swap, to_encrypt, rng);

	//shows what the scrambled message looks like
	cv::namedWindow("post Scramble");
	cv::imshow("post Scramble", to_encrypt);
	cv::imwrite("../Visual/Part_B_encode_Scabled.png", to_encrypt);

	//encryptes it into carrier image
	encrypt(carrier, to_encrypt);
	
	//waits for key press then destroys all windows.
	cv::waitKey(0);
	cv::destroyAllWindows();
	
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

void encrypt(cv::Mat& carrier, cv::Mat& to_encrypt) {
	//inverst then changes scale (scale needs to be 0-1 instead of 0-255)
	to_encrypt = (255 - to_encrypt) / 254;

	cv::Mat encoded = carrier + to_encrypt;

	cv::namedWindow("encoded");
	cv::imshow("encoded", encoded);

	//saves encoded image.
	cv::imwrite("../outputs/Part_B_encoded.png", encoded);
}

void flatten_shuffle(std::vector<cv::Point2i>& flat_coord, std::vector<int>& shuffle_order, cv::Mat& img, cv::RNG& rng) {
	//get the swap index and the flat 
	int idx; //is the random idx flat_coord will swap with
	for (int x = 0; x < img.rows; x++) {
		for (int y = 0; y < img.cols; y++) {
			flat_coord.push_back(cv::Point2i(x, y));
			idx = rng.uniform(1, img.cols * img.rows);
			shuffle_order.push_back(idx);
		}
	}

	//shuffle coords
	for (int i = 0; i < flat_coord.size(); i++) {
		cv::Point2i temp = flat_coord[i]; //when swapping this value saves what is lost
		flat_coord[i] = flat_coord[shuffle_order[i]]; //sets point 1 to be point 2
		flat_coord[shuffle_order[i]] = temp; // sets point 2 to be what point 1 was through the temp saved value
	}

	//channel pixels around
	int temp;
	idx = 0;
	for (int x = 0; x < img.rows; x++) {
		for (int y = 0; y < img.cols; y++) {
			//goes through each pixel and index to swap with and swaps them around -> actual scramblling takes place here

			temp = img.at<uchar>(x, y); //saves point 1s information
			img.at<uchar>(x, y) = img.at<uchar>(flat_coord[idx].x, flat_coord[idx].y); //swaps point 1 with point 2 information
			img.at<uchar>(flat_coord[idx].x, flat_coord[idx].y) = temp; //sets point 2s information back to what point 1 was
			idx++; //move on to the next point
		}
	}
}

