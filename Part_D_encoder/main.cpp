/*	this is the encoder for Part D from the image processing analysis assignemtn
*	
*	This program is able to take a colord image and scrables, adds noise and encodes 
*	it in a nother image for it not to be seen.
*	
*	to run this just run the exe in the command promt with the parameters of the 
*	image path that you want as a carrier and the image path of the image you want
*	to encrypt. it will ask for a pass word to set so that when decoding it can 
*	see you have access to the message 
*	
*	hash funciiton isnt mine site on where to find it can be found in the function
*	
*	 bare in mind that this part my be abiit slow with larger images as its got litteral thousands of operations to do
*/
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

//declaration for all function
cv::Mat make_noise(cv::Mat& to_encrypt, cv::RNG& rng, double sigma);
void add_noise(cv::Mat& img, cv::Mat& noise);
void flatten_shuffle(std::vector<cv::Point3i>& flat_coord, std::vector<int>& swap_coord,
					 cv::RNG& rng, cv::Mat& img);
void encode(cv::Mat& carrier, cv::Mat& to_encrypt);
unsigned long hash(const char *str);

int main(int argc, char** argv) {
	// if doesnt have the right arguments it doesnt run.
	if (argc != 3) {
		std::cout << "Not the right number of ar" << std::endl; //displays message 
		return -1; //terminates
	}

	//loads and checks image
	cv::Mat carrier= cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
	if (carrier.empty()) {
		std::cout << "carrier img cant be loaded, check if path is correct " << std::endl; //disalys message
		return -1; //terminates 
	}

	//load and chaks image
	cv::Mat to_encrypt = cv::imread(argv[2], CV_LOAD_IMAGE_COLOR);
	if (carrier.empty()) {
		std::cout << " img to encrypt cant be loaded, check if path is correct " << std::endl; //display error message
		return -1; //terminates 
	}

	//makes password
	std::string pass;
	std::cout << "please give your password: ";
	std::getline(std::cin, pass);
	unsigned long hash_number = hash(pass.c_str()); //gets a seend number

	//sets rng using seed number
	cv::RNG rng(hash_number); 

	// makes noise usng rng class 
	cv::Mat noise = make_noise(to_encrypt, rng, 10.0);

	//inverts image to encrypt
	to_encrypt = (cv::Vec3b(255, 255, 255) - to_encrypt);

	//adds noise to the image to encrypt 
	add_noise(to_encrypt, noise);

	//initializes vectors to store flat image and indexs to swap coords wiith
	std::vector<cv::Point3i> flat_coords;
	std::vector<int> swap_coords;

	//before shuffle show
	cv::namedWindow("shuffle_before");
	cv::imshow("shuffle_before", to_encrypt);
	cv::imwrite("../Visual/Part_D_encoder_visual/to_encrypt_before_shuffle.png",to_encrypt);

	//flattens and shuffles image
	flatten_shuffle(flat_coords, swap_coords, rng, to_encrypt);
	cv::namedWindow("shuffle_After");
	cv::imshow("shuffle_After", to_encrypt);
	cv::imwrite("../Visual/Part_D_encoder_visual/to_encrypt_after_shuffle.png",to_encrypt);

	//encrypts into carrier img
	encode(carrier, to_encrypt);
	cv::namedWindow("after encrypt");
	cv::imshow("after encrypt", carrier);
	

	//waits for key press
	cv::waitKey(0);
	cv::destroyAllWindows();

	//saves image in this location
	cv::imwrite("../outputs/Part_D_encoded.png", carrier);
	cv::imwrite("../Visual/Part_D_encoder_visual/Part_D_encoded.png",carrier);

	return 0;
}


cv::Mat make_noise(cv::Mat& to_encrypt, cv::RNG& rng, double sigma) {
	//makes noise from RNG with given seed
	cv::Mat emtpy(to_encrypt.rows, to_encrypt.cols, CV_8UC3, cv::Scalar(0, 0, 0)); //sets a emtpy image to put noise
	
	//goes through all channels and pixels and sets a value
	for (int x = 0; x < emtpy.rows; x++) {
		for (int y = 0; y < emtpy.cols; y++) {
			for (int i = 0; i < 3; i++) {
				int val = emtpy.at<cv::Vec3b>(x, y)[i];
				int gaus =  rng.gaussian(sigma);
				
				//deals with over flow (doesnt really matter now that i think of it)... yes im abit lazy to change it right now (hope that made you laugh)
				if((val + gaus) >255){
					emtpy.at<cv::Vec3b>(x, y)[i] = 255;
				}
				else {
					emtpy.at<cv::Vec3b>(x, y)[i] = val + gaus;
				}
			}
		}
	}
	//returns noise
	return emtpy;
}

void add_noise(cv::Mat& img, cv::Mat& noise) {
	//adds noise to the image
	// goes through all pixels and channels and adds noise to image 
	for (int x = 0; x < img.rows; x++) {
		for (int y = 0; y < img.cols; y++){
			for (int chan = 0; chan < 3; chan++) {
				int img_val = img.at<cv::Vec3b>(x, y)[chan];
				int noise_val = noise.at<cv::Vec3b>(x, y)[chan];
				
				//deals with over flow
				//if pixel is over 255, jsut stes the cap to 255.
				if ((img_val + noise_val) > 255) {
					img.at<cv::Vec3b>(x, y)[chan] = 255;
				}
				else {
					img.at<cv::Vec3b>(x, y)[chan] = img_val + noise_val;
				}
			}
		}
	}
}

void flatten_shuffle(std::vector<cv::Point3i>& flat_coord, std::vector<int>& swap_coord,
					 cv::RNG& rng, cv::Mat& img) 
{
	//this function flattens the image into a 1d vector and swaps it around using the swap coords
	
	//makes the flattened image and vector of idexes for swapping 
	int idx;
	for (int x = 0; x < img.rows; x++) {
		for (int y = 0; y < img.cols; y++) {
			for (int i = 0; i < 3; i++) {
				flat_coord.push_back(cv::Point3i(x, y, i)); //gives x,y and channel
				idx = rng.uniform(0, img.rows*img.cols * 3); //gives random number from 0 to number of points there are 
				swap_coord.push_back(idx); //saves random index to swap to
			}
		}
	}

	//swaps the flattned image, used later for swapping pixels in actual image
	for (int i = 0; i < flat_coord.size(); i++) {
		cv::Point3i temp = flat_coord[i]; //saves point 1
		flat_coord[i] = flat_coord[swap_coord[i]]; // puts point 2 into point 1
		flat_coord[swap_coord[i]] = temp; //puts point 1 into point 2
	}

	//swaps values in actual image
	idx = 0; 
	int temp;
	for (int x = 0; x < img.rows; x++) {
		for (int y = 0; y < img.cols; y++) {
			for (int i = 0; i < 3; i++) {
				//goes throug each pixel and channel
				temp = img.at<cv::Vec3b>(x, y)[i]; //saves value for point 1
				img.at<cv::Vec3b>(x, y)[i] = img.at<cv::Vec3b>(flat_coord[idx].x, flat_coord[idx].y)[flat_coord[idx].z];// puts point 2 into point 1
				img.at<cv::Vec3b>(flat_coord[idx].x, flat_coord[idx].y)[flat_coord[idx].z] = temp; //sets point 1 into point 2
				idx++; //increases idex of where to find the index to swap
			}
		}
	}
}

void encode(cv::Mat& carrier, cv::Mat& to_encrypt) {
	//goes through every pixel and channel and shrinks down scale to 0 - 1 
	for (int x = 0; x < to_encrypt.rows; x++) {
		for (int y = 0; y < to_encrypt.cols; y++) {
			for (int i = 0; i < 3; i++) {
				to_encrypt.at<cv::Vec3b>(x, y)[i] = to_encrypt.at<cv::Vec3b>(x, y)[i] / 255;
			}
		}
	}
	//adds the encrypted to carrier (only small changes happen)
	carrier = carrier + to_encrypt;
}

unsigned long hash(const char *str)
{
	// this fucntions isnt mine, can be fine here http://www.cse.yorku.ca/~oz/hash.html
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}