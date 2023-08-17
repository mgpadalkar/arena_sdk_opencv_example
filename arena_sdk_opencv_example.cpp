#include "iostream"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

#include "ArenaApi.h"
#include "SaveApi.h"

std::string print_ip(uint32_t ip)
{
	unsigned char bytes[4];
	bytes[0] = ip & 0xFF;
	bytes[1] = (ip >> 8) & 0xFF;
	bytes[2] = (ip >> 16) & 0xFF;
	bytes[3] = (ip >> 24) & 0xFF;

	return std::to_string(bytes[3]) + "." + std::to_string(bytes[2]) + "." + std::to_string(bytes[1]) + "." + std::to_string(bytes[0]);
}


int main(int argc, char* argv[])
{
    // get attached devices
    Arena::ISystem* pSystem = Arena::OpenSystem();
    pSystem->UpdateDevices(100);
    std::vector<Arena::DeviceInfo> deviceInfos = pSystem->GetDevices();
    if (deviceInfos.size() == 0)
    {
    	std::cout << "\nNo camera connected\nPress enter to complete\n";
    	std::getchar();
    	return 0;
    }

    // select first device
    Arena::IDevice* pDevice = pSystem->CreateDevice(deviceInfos[0]);

    // get name
    GenApi::INodeMap *pNodeMap = pDevice->GetNodeMap();
    std::string deviceSerialNumber = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "DeviceSerialNumber").c_str();
    std::string deviceModelName = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "DeviceModelName").c_str();

    // get IP address
    auto pIfaceNodeMap = pSystem->GetTLInterfaceNodeMap(deviceInfos[0]);
    GenApi::CIntegerPtr pIntIp = pIfaceNodeMap->GetNode("GevInterfaceSubnetIPAddress");
    uint32_t ifIp = static_cast<uint32_t>(pIntIp->GetValue());
    std::string current_IP_address = print_ip(ifIp);

    // create window name for display
    std::string windowName = deviceModelName + " (S/N: " + deviceSerialNumber + ")  | (Current IP: " + current_IP_address + ") ";
    std::cout << "Found " + windowName + "!" << std::endl;

    // create image pointers
    Arena::IImage *pImage;
    cv::Mat img;
    
    // set timeout
    GenApi::CFloatPtr pExposureTimeNode = pNodeMap->GetNode("ExposureTime");
    const int64_t getImageTimeout_ms = static_cast<int64_t>(pExposureTimeNode->GetMax() / 1000 * 2);

    // // get acquisition controls
    // int framesPerSecond = (int)Arena::GetNodeValue<double>(pNodeMap, "AcquisitionFrameRate");
    // int numberOfSeconds = 10;
    // int numFramesToAcquire = framesPerSecond * numberOfSeconds;
    // // for (int i = 0; i < numFramesToAcquire;  i++)


    // start acquiring in the loop
    std::cout << "Starting stream." << std::endl;
    pDevice->StartStream();
    cv::namedWindow(windowName.c_str(), cv::WINDOW_NORMAL);
    while(1)
    {
        pImage = pDevice->GetImage(getImageTimeout_ms);
        // img = cv::Mat((int)pImage->GetHeight(), (int)pImage->GetWidth(), CV_8UC1, (void *)pImage->GetData());
        auto pConverted = Arena::ImageFactory::Convert(pImage, BGR8); 
        img = cv::Mat((int)pImage->GetHeight(), (int)pImage->GetWidth(), CV_8UC3, (void *)pConverted->GetData());

        cv::imshow(windowName.c_str(), img);
        int key_pressed = cv::waitKey(1) & 0xFF;
        pDevice->RequeueBuffer(pImage);
        if (key_pressed == 'q' or key_pressed == 27)
            break;
    }
    cv::destroyAllWindows();

    // Stop stream
    std::cout << "Stopping stream." << std::endl;
    pDevice->StopStream();

    return 0;
}
