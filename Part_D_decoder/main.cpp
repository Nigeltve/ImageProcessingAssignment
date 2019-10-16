/*	this is the decoder to part D in the image processing analysis assignment
*	what this does is take a pass word to generate the noise and unscrable a colored 	
*	encoded image
*	
*	this has some issues as it does do it 100% accurate but is good enough to read the message
*	
*	to run this you need to run the encoder first 
*	in the command promt run the exe with the original image and the image to decode
*	it'll ask for the a pass word, if its right youll get a message, if its wrong
*	you jsut get noise and cannot read the message
*	
*	the hash fucntion is not mine, original can be found on the site in the function
*	lecturer said its ok to use it.
*	
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

cv::Mat decode(cv::Mat& original, cv::Mat& to_decode);
unsigned long hash(const char *str);
cv::Mat make_noise(cv::Mat& to_denoise, cv::RNG& rng, double sigma);
void flatten_shuffle_Back(std::vector<cv::Point3i>& flat_coord, std::vector<int>& swap_coord,
	cv::RNG& rng, cv::Mat& img);
void thresh(cv::Mat& img);

int main(int argc, char** argv) {

	//sees if number of arguments are right
	if (argc != 3) {
		std::cout << "the number of arguments was not right please put in the right arguments" << std::endl;
		return -1;
	}

	//gets the original img
	cv::Mat original = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
	if (original.empty()) {
		std::cout << "could not load origianl image for decoding" << std::endl;
		return -1;
	}

	//gets imgage to decode
	cv::Mat to_decode = cv::imread(argv[2], CV_LOAD_IMAGE_COLOR);
	if (to_decode.empty()) {
		std::cout << "could not load the image to decode" << std::endl;
		return - 1;
	}

	//sets password
	std::string pass;
	std::cout << "please input your pass word: ";
	std::getline(std::cin, pass);

	//displays the img to decode
	cv::namedWindow("to decode");
	cv::imshow("to decode", to_decode);

	//sets rng
	unsigned long hash_number = hash(pass.c_str());
	cv::RNG rng(hash_number);
	
	//makes noise img
	cv::Mat noise = make_noise(to_decode, rng, 10.0);


	//does the decoding
	to_decode = decode(original, to_decode);

	cv::imwrite("../Visual/Part_D_decoder_visual/step_1_decoded.png", to_decode);

	//shuffles and swaps;
	std::vector<cv::Point3i> flat_coords;
	std::vector<int> shuffle_point;
	flatten_shuffle_Back(flat_coords, shuffle_point, rng, to_decode);

	//displlays img
	cv::namedWindow("shuffled back");
	cv::imshow("shuffled back", to_decode);
	cv::imwrite("../Visual/Part_D_decoder_visual/step_2_uncrambled.png", to_decode);
	
	//subtracks noise from img,
	cv::Mat decoded;
	cv::subtract(to_decode, noise, decoded);
	cv::imwrite("../Visual/Part_D_decoder_visual/step_3_subtracted.png", decoded);


	//threshholds it for cleaining it up
	thresh(decoded);
	cv::imwrite("../Visual/Part_D_decoder_visual/step_4_thresh.png", decoded);

	//inverts it
	decoded = (cv::Vec3b(255, 255, 255) - decoded);
	cv::imwrite("../Visual/Part_D_decoder_visual/step_5_inverted.png", decoded);


	//displays noise
	cv::namedWindow("decoded");
	cv::imshow("decoded", decoded);
	cv::imwrite("../Visual/Part_D_decoder_visual/step_6_full_decoded.png", decoded);
	//waits for key press
	cv::waitKey(0);
	cv::destroyAllWindows();

	//saves img
	cv::imwrite("../outputs/Part_D_decoded.png", decoded);

	return 0;
}

cv::Mat decode(cv::Mat& original, cv::Mat& to_decode) {
	//gets the scrambled message out of the original
	cv::Mat decoded = to_decode - original;

	//scales it up so its not in range 0 - 1.
	for (int x = 0; x < decoded.rows; x++) {
		for (int y = 0; y < decoded.cols; y++) {
			for (int i = 0; i < 3; i++) {
				decoded.at<cv::Vec3b>(x, y)[i] = decoded.at<cv::Vec3b>(x, y)[i] * 255;
			}
		}
	}

	//returns decoded scrambled message
	return decoded;
}

cv::Mat make_noise(cv::Mat& to_denoise, cv::RNG& rng, double sigma) {
	//from the password it generages the enoise needed to get rid of in the encoded image
	cv::Mat emtpy(to_denoise.rows, to_denoise.cols, CV_8UC3, cv::Scalar(0, 0, 0));
	//gos through each pixel in empty image and gives it a gauseian number 
	for (int x = 0; x < emtpy.rows; x++) {
		for (int y = 0; y < emtpy.cols; y++) {
			for (int i = 0; i < 3; i++) {
				int val = emtpy.at<cv::Vec3b>(x, y)[i];
				int gaus = rng.gaussian(sigma);

				//deals with overflow
				if ((val + gaus) > 255) {
					emtpy.at<cv::Vec3b>(x, y)[i] = 255;
				}
				else {
					emtpy.at<cv::Vec3b>(x, y)[i] = val + gaus;
				}
			}
		}
	}
	return emtpy;
}


void flatten_shuffle_Back(std::vector<cv::Point3i>& flat_coord, std::vector<int>& swap_coord,
						  cv::RNG& rng, cv::Mat& img)
{
	//this does the same as B but it takes color into account by adding it to the flatten points
	// flat vector looks like this [(x1,y1,c1),(x1,y1,c2),(x1,y1,c3) (x2,y2,c1),(x2,y2,c2),(x2,y2,c3) ... (xn,yn,c1),(xn,yn,c2),(xn,yn,c3)]

	//flatten and swap
	int idx; //what random number will be
	for (int x = 0; x < img.rows; x++) {
		for (int y = 0; y < img.cols; y++) {
			for (int i = 0; i < 3; i++) {
				flat_coord.push_back(cv::Point3i(x, y, i)); //pushes back x y and the color channel
				idx = rng.uniform(0, img.rows*img.cols * 3); //gives a random value from 0 to how big the flat vector will be
				swap_coord.push_back(idx); //puts the random vale in to vector for later
			}
		}
	}

	//swaps;
	for (int i = 0; i < flat_coord.size(); i++) {
		cv::Point3i temp = flat_coord[i]; //saves point 1
		flat_coord[i] = flat_coord[swap_coord[i]]; // puts point 2 in point 1
		flat_coord[swap_coord[i]] = temp; //puts point 1 int point 2
	}

	//goes from the back to "unmix pixels 
	idx = flat_coord.size()-1; //end of shuffle vector
	int temp; //what we save valeu in
	for (int x = img.rows -1 ; x >= 0; x--) {
		for (int y = img.cols -1 ; y >= 0; y--) {
			for (int i = 2; i >= 0; i--) {
				//goes backwards startign from the bottem left 3rd channel 
				temp = img.at<cv::Vec3b>(x, y)[i]; //saves point 1
				img.at<cv::Vec3b>(x, y)[i] = img.at<cv::Vec3b>(flat_coord[idx].x, flat_coord[idx].y)[flat_coord[idx].z]; // sets point2 in point 1
				img.at<cv::Vec3b>(flat_coord[idx].x, flat_coord[idx].y)[flat_coord[idx].z] = temp; //sets point 1 with point 2
				idx--; //takes a step back.
			}
		}
	}
}

void thresh(cv::Mat& img) {
	//tries to clear up some noise 
	//goes though every pixel and chennel
	//	if not 0 sets it to 255 (white)
	//	else tries to set it to black.
	for (int x = 0; x < img.rows; x++) {
		for (int y = 0; y < img.cols; y++) {
			for (int chan = 0; chan < 3; chan++) {
				if (img.at<cv::Vec3b>(x, y)[chan] != 0) {
					img.at<cv::Vec3b>(x, y)[chan] = 255;
				}
				else {
					img.at<cv::Vec3b>(x, y)[chan] = 0;
				}
			}
		}
	}
}

unsigned long hash(const char *str)
{
	// this is not mine, can be found here http://www.cse.yorku.ca/~oz/hash.htm
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}
