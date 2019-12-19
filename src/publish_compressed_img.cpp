#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <string>
#include <iostream>
#include <fstream>
#include <rosbag/bag.h>
#include <sensor_msgs/image_encodings.h>
#include "compressed_image_transport/compressed_publisher.h"
#include "compressed_image_transport/compression_common.h"
namespace enc = sensor_msgs::image_encodings;
using namespace std;
using namespace compressed_image_transport;

sensor_msgs::CompressedImage compresses(sensor_msgs::ImagePtr msg){
    sensor_msgs::CompressedImage compressed;
    compressed.header = msg->header;
    compressed.format = msg->encoding;

    // Compression settings
    std::vector<int> params;

    // Get codec configuration
    
    compressionFormat encodingFormat = PNG;
    // Bit depth of image encoding
    int bitDepth = enc::bitDepth(msg->encoding);
    int numChannels = enc::numChannels(msg->encoding);

    params.resize(3, 0);
    params[0] = cv::IMWRITE_PNG_COMPRESSION;
    params[1] = 9;    //config_.png_level [1,9]

    // Update ros message format header
    compressed.format += "; png compressed ";

    // Check input format
    if ((bitDepth == 8) || (bitDepth == 16))
    {

        // Target image format
        stringstream targetFormat;
        if (enc::isColor(msg->encoding))
        {
            // convert color images to RGB domain
            targetFormat << "bgr" << bitDepth;
            compressed.format += targetFormat.str();
        }

        // OpenCV-ros bridge
        try
        {
            boost::shared_ptr<CompressedPublisher> tracked_object;
            cv_bridge::CvImageConstPtr cv_ptr = cv_bridge::toCvShare(*msg, tracked_object, targetFormat.str());

            // Compress image
            if (cv::imencode(".png", cv_ptr->image, compressed.data, params))
            {

            float cRatio = (float)(cv_ptr->image.rows * cv_ptr->image.cols * cv_ptr->image.elemSize())
                / (float)compressed.data.size();
            ROS_DEBUG("Compressed Image Transport - Codec: png, Compression Ratio: 1:%.2f (%lu bytes)", cRatio, compressed.data.size());
            }
            else
            {
            ROS_ERROR("cv::imencode (png) failed on input image");
            }
        }
        catch (cv_bridge::Exception& e)
        {
            ROS_ERROR("%s", e.what());
            return compressed;
        }
        catch (cv::Exception& e)
        {
            ROS_ERROR("%s", e.what());
            return compressed;
        }
    }
    else
        ROS_ERROR("Compressed Image Transport - PNG compression requires 8/16-bit encoded color format (input format is: %s)", msg->encoding.c_str());
    return compressed;
}
int main(int argc, char** argv)
{
    ros::init(argc, argv, "image_transport");
    ros::NodeHandle nh;

    string path="image.jpg";
    cv::Mat image = cv::imread(path, CV_LOAD_IMAGE_COLOR);
    sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();
    sensor_msgs::CompressedImage compressed;
    compressed = compresses(msg);
    rosbag::Bag bag;
    bag.open("/data2/nuScenes/image_test/test_comp.bag", rosbag::bagmode::Write);
    bag.write("image/compressed",ros::Time::now(),compressed);
    bag.close();

}
