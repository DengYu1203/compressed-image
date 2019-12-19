# compressed-image
Using image_transport(ROS pkg) to compress and decompress image
1. compress the raw image with png-compressed method(png level=9), because the jpeg-compressed would casue distortion
2. decompress the compressed image to raw image

## source
1. [image_transport ROS wiki](http://wiki.ros.org/image_transport)
2. [compressed_image_transport](http://wiki.ros.org/compressed_image_transport)

## background
I need to get the sensor_msgs::CompressedImage msg from sensor_msgs::Image msg directly because I want to write the sensor_msgs::CompressedImage msg into rosbag.
However, the image_transport package cannot directly transform the raw image to compressed image.

## method
> Make sure that the *compressed_image_transport* and *image_transport* are installed.

I use the [compressed_image_transport](http://wiki.ros.org/compressed_image_transport) to get the [sensor_msgs::CompressedImage msg](src/publish_compressed_img.cpp).
In addition, I also write the file to convert the compressedImage to [sensor_msgs::Image msg](src/subscribe_compressed_img.cpp).

## roslaunch file
Using the image_transpot package to get the multiple raw images from the compressed images directly.
