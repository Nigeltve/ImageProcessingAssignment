/*	this is the decoder for Part B of the image processing analysis assignment
*	
*	here it finds the message from the Part B encoder by finding the scrabled message
*	and with the right pass word able to relocate every pixel to the correct place 
*	to give the message.
*	
*	to use go to exe and give it your original and encoded, run it and it will ask for password
*	if the password is right, it will give the right message back. else just looks like random noise
*	
*	the hash_djb2 is not mine, given to us by website (stated inside funciton), lectuere said
*	we can use it.
*	
*/


#include<iostream>
#include <opencv2/opencv.hpp>
#include <vector>

unsigned long hash_djb2(const char* str);
cv::Mat decode(cv::Mat& carrier, cv::Mat& to_encrypt);
void flatten_shuffle_back(std::vector<cv::Point2i>& flat_coord, std::vector<int>& shuffle_order, cv::Mat& img, cv::RNG& rng);

int main(int argc, char** argv) {
	
	//if not right number of arguments given terminates program
	if (argc != 3) {
		//displays message
		std::cout << "you do not have the right amount of arguments" << std::endl;
		return -1;
	}
	//asks for password
	std::cout << "plaese put in your password: ";
	std::string password;
	std::getline(std::cin, password);
	unsigned long hash_number = hash_djb2(password.c_str()); //gets seed number

	cv::RNG rng(hash_number); //sets rng class with seed

	//sets the files to help the decode  process
	cv::Mat original, to_decode, message;
	std::vector<cv::Point2i> flat;
	std::vector<int> swap;

	//sees if fiels are there
	//if not there it will display the message and terminate the program
	original = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if (original.empty()) { std::cout << "original img is emtpy (path might be wrong)" << std::endl; return -1; } 
	to_decode = cv::imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
	if (to_decode.empty()) { std::cout << "image to decode is emtpy (path might be wrong)" << std::endl; return -1; }

	//decodes the scrabled message 
	message = decode(original, to_decode);
	//does the actual unscrabling
	flatten_shuffle_back(flat, swap, message, rng);

	//waits for keypress 
	cvWaitKey(0);
	cv::destroyAllWindows();
	
	//saves message
	cv::imwrite("../outputs/Part_B_decoded.png", message);
	std::cout << "saved " << std::endl;
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

cv::Mat decode(cv::Mat& carrier, cv::Mat& to_dencrypt) {
	cv::Mat decoded = 255 - (to_dencrypt - carrier) * 255; //does the oposite of what the encoder did

	//displays what came out.
	cv::namedWindow("decoded but scrambled");
	cv::imshow("decoded but scrambled", decoded);
	
	return decoded;
}

void flatten_shuffle_back(std::vector<cv::Point2i>& flat_coord, std::vector<int>& shuffle_order, cv::Mat& img, cv::RNG& rng) {
	// this reconscruts the vector in which to swap the pixel with.
	// goes instead of going forward, it just goes back ward to be 
	//able to palce the pixels in the right order to display message

	int idx; //this is what randomd number will be sored 
	for (int x = 0; x < img.rows; x++) {
		for (int y = 0; y < img.cols; y++) {
			// goes through each pixel and puts it into one vector
			// each time it does this, it gives a "random" number to the shuffle order
			flat_coord.push_back(cv::Point2i(x, y));
			idx = rng.uniform(1, img.cols * img.rows);
			shuffle_order.push_back(idx);
		}
	}


	//shuffle coords into correct order
	for (int i = 0; i < flat_coord.size(); i++) {
		cv::Point2i temp = flat_coord[i]; //saves point 1
		flat_coord[i] = flat_coord[shuffle_order[i]]; //saves point2 coords into point 1
		flat_coord[shuffle_order[i]] = temp; //point 1 info was lost lucky we saved it -> saves point 1 into point 2
	}

	//using samve idea as loop above but with the actual image
	//also goes back for from essentiall "unmixing" pixels
	int temp;
	idx = flat_coord.size()-1; 
	for (int x = img.rows - 1; x >= 0; x--) {
		for (int y = img.cols - 1; y >= 0; y--) {
			temp = img.at<uchar>(x, y); //saves point 1
			img.at<uchar>(x, y) = img.at<uchar>(flat_coord[idx].x, flat_coord[idx].y); //sets point 1 to be point 2
			img.at<uchar>(flat_coord[idx].x, flat_coord[idx].y) = temp; //sets point 2 to be point 1
			idx--;
		}
	}
	//displays img.
	cv::namedWindow("unscrambled");
	cv::imshow("unscrambled", img);
	cv::imwrite("../Visual/Part_B_decoder_visual/Unscrambled.png", img);
	std::cout << "saved unscram" << std::endl;
}

