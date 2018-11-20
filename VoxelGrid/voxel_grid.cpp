#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/io/io.h>
#include <stdio.h>


int user_data;

int count(1);


void
viewerOneOff(pcl::visualization::PCLVisualizer& viewer)
{
	viewer.setBackgroundColor(1.0, 0.5, 1.0);
	pcl::PointXYZ o;
	o.x = 1.0;
	o.y = 0;
	o.z = 0;
	viewer.addSphere(o, 0.25, "sphere", 0);
	std::cout << "i only run once" << std::endl;

}

void
viewerPsycho(pcl::visualization::PCLVisualizer& viewer)
{
	static unsigned count = 0;
	std::stringstream ss;
	ss << "Once per viewer loop: " << count++;
	viewer.removeShape("text", 0);
	viewer.addText(ss.str(), 200, 300, "text", 0);

	//FIXME: possible race condition here:
	user_data++;
}

int
main(int argc, char** argv)
{
	pcl::PCLPointCloud2::Ptr cloud(new pcl::PCLPointCloud2());
	pcl::PCLPointCloud2::Ptr cloud_filtered(new pcl::PCLPointCloud2());
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr beforercloud(new pcl::PointCloud<pcl::PointXYZRGBA>);
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr aftercloud(new pcl::PointCloud<pcl::PointXYZRGBA>);

	// Fill in the cloud data
	pcl::PCDReader reader;
	// Replace the path below with the path where you saved your file
	reader.read("test_pcd.pcd", *cloud); // Remember to download the file first!

	std::cerr << "PointCloud before filtering: " << cloud->width * cloud->height
		<< " data points (" << pcl::getFieldsList(*cloud) << ").\n";

	// Create the filtering object
	pcl::VoxelGrid<pcl::PCLPointCloud2> sor;
	sor.setInputCloud(cloud);
	sor.setLeafSize(0.01f, 0.01f, 0.01f);
	sor.filter(*cloud_filtered);

	std::cerr << "PointCloud after filtering: " << cloud_filtered->width * cloud_filtered->height
		<< " data points (" << pcl::getFieldsList(*cloud_filtered) << ")." <<std::endl;

	pcl::PCDWriter writer;
	writer.write("test_downsampled.pcd", *cloud_filtered,
		Eigen::Vector4f::Zero(), Eigen::Quaternionf::Identity(), false);


	
	pcl::io::loadPCDFile("test_pcd.pcd", *beforercloud);
	pcl::io::loadPCDFile("test_downsampled.pcd", *aftercloud);
	

	
	while(count){
	
		
		int a;
		std::cout << "숫자를 입력하세요 : ";
		std::cin >> a;
		pcl::visualization::CloudViewer viewer("Cloud Viewer");
		viewer.runOnVisualizationThreadOnce(viewerOneOff);

		switch (a)
		{
		case 0:

			viewer.showCloud(beforercloud);

			viewer.runOnVisualizationThread(viewerPsycho);
			while (!viewer.wasStopped())
			{
				user_data++;
			}
			break;

		case 1:

			viewer.showCloud(aftercloud);

			viewer.runOnVisualizationThread(viewerPsycho);
			while (!viewer.wasStopped())
			{
				user_data++;
			}

			break;
		
		default :

			count=0;
			break;

			
		}
	}
	
	
	

	return (0);
}