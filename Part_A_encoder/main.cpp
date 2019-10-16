/*	this is the Part A encoder for the image processing analysis assignment 
*	
*	to run this, open the command prompt and head to the folder with the exe
*	run it with the paths to the carrier and the message iamges, then press enter
*	to should do the rest for you.
*	
*	
*/

#include <opencv2/opencv.hpp>
#include <iostream>

void encode(cv::Mat &carry, cv::Mat &to_encrypt);

int main(int argc, char** argv) {
	for (int i = 0; i < argc; i++) {
		// shows what arguments were being passed in
		std::cout << "item " << i << ": " << argv[i] << std::endl;
	}

	cv::Mat carrier, to_encrypt; //init for all the image matricies

	if (argc != 3) {
		//checks to see if the correct number of inputs were given
		std::cout << "Error: not the right number of aruments given, please try again." << std::endl;
		return -1;
	}

	carrier = cv::imread(argv[1]); // sets the carrier image
	to_encrypt = cv::imread(argv[2]); // sets the image you want to encrypt

	if (carrier.empty()) {
		//checks to see if carrier is emtpy
		std::cout << "carrier image is empty" << std::endl;
		return -1;
	}

	if (to_encrypt.empty()) {
		//checks to see if carrier is emtpy
		std::cout << "the image to encrypt is empty" << std::endl;
		return -1;
	}

	//does encoding if image
	std::cout << "encoding" << std::endl;
	encode(carrier, to_encrypt);

	return 0;
}

void encode(cv::Mat &carry, cv::Mat &to_encrypt) {
	/* this functions 2 images one to be the carrier for the img that is going to be encoded
	 * and saves it into encoded
	 *
	 * @param carry: this is the carrier img
	 * @param to_encrypt: this is the img to be encrypted
	 * @param encoded: where the encoded img is going to be stored
	 * @poram verbose: 0 means doesnt show the images		 1 means that it does show images
	*/
	cv::cvtColor(carry, carry, cv::COLOR_BGR2GRAY);
	cv::cvtColor(to_encrypt, to_encrypt, cv::COLOR_BGR2GRAY);

	//inverst it and scales it down
	to_encrypt = (255 - to_encrypt);
	cv::imwrite("../Visual/to_Encrypt_Encoder_A_inv.png", to_encrypt);

	to_encrypt = to_encrypt/255;
	cv::imwrite("../Visual/to_Encrypt_Encoder_A_scaleDown.png",to_encrypt);


	//makes the encoded image by addign the encrypted to the carrier.
	cv::Mat encoded = carry + to_encrypt;

	//writes file.
	cv::imwrite("../outputs/Part_A_encoded.png", encoded);

}