/*	this is the decoder for part A in the immage processing analysis assignment
*	
*	this program in the command prompt takes the exe original image and encoded 
*	image and find the message hidden inside the encoded image.
*	
*/
#include <iostream>
#include <opencv2/opencv.hpp>

void decode(cv::Mat &original, cv::Mat &encrytped);

int main(int argc, char** argv) {
	for (int i = 0; i < argc; i++) {
		// shows what arguments were being passed in
		std::cout << "item " << i << ": " << argv[i] << std::endl;
	}

	cv::Mat Original, decoded, encoded; //init for all the image matricies

	if (argc != 3) {
		//checks to see if the correct number of inputs were given
		std::cout << "Error: not the right number of aruments given, please try again." << std::endl;
		return -1;
	}

	Original = cv::imread(argv[1]); // sets the carrier image
	encoded = cv::imread(argv[2]); //sets the encoded img

	if (Original.empty()) {
		//checks to see if carrier is emtpy
		std::cout << "carrier image is empty" << std::endl;
		return -1;
	}

	if (encoded.empty()) {
		//checks to see if carrier is emtpy
		std::cout << "the image to encrypt is empty" << std::endl;
		return -1;
	}

	//does decoding of image
	std::cout << "decoding" << std::endl;
	decode(Original, encoded);

	cv::destroyAllWindows();
	return 0;
}


void decode(cv::Mat &original, cv::Mat &encrytped) {
	/* gets the image that was encoded from the carrier image if verbose is set to 0
	 * it doesnt show images, if set to 1 it shows images.
	 *
	 * @param original: what the original image looks like
	 * @param encrytped: image that has encrypted data
	*/

	cv::cvtColor(encrytped,encrytped,cv::COLOR_BGR2GRAY);
	cv::cvtColor(original,original,cv::COLOR_BGR2GRAY);

	cv::Mat decoded = 255 - (encrytped - original)*255; //does the oposite of what the encoder did

	cv::Mat visual = encrytped - original;
	cv::imwrite("../Visual/decoded_A_encrypt_Sub_Original.png", visual);
	
	visual = visual*255;
	cv::imwrite("../Visual/decoded_A_scaled_up.png",visual);

	visual = 255 - visual;
	cv::imwrite("../Visual/decoded_A_inv.png", visual);


	
	//writes a file what the image is
	cv::imwrite("../outputs/Part_A_decoded.png", decoded);
}