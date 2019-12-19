# compressed-image
using image_transport(ROS pkg) to decompress image

## image_transport package
[image_transport ROS wiki](http://wiki.ros.org/image_transport)
Using the image_transport to compress the raw image with png-compressed method(png level=9)

Due to the package cannot directly transform the raw image to compressed image, I use the [compressed_image_transport](http://wiki.ros.org/compressed_image_transport) to get the [sensor_msgs::CompressedImage msg](src/publish_compressed_img.cpp).
In addition, I also write the file to convert the compressed image to [sensor_msgs::Image msg](src/subscribe_compressed_img.cpp).

