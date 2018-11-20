#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/io/io.h>
#include <stdio.h>
#include <pcl/filters/statistical_outlier_removal.h>


int user_data;

bool count(true);

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
	pcl::PointCloud<pcl::PointXYZ>::Ptr before(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr after(new pcl::PointCloud<pcl::PointXYZ>);

	pcl::PointCloud<pcl::PointXYZ>::Ptr downsampleoutliers(new pcl::PointCloud<pcl::PointXYZ>);

	//pcl::PCLPointCloud2::Ptr before_sample(new pcl::PCLPointCloud2());
	//pcl::PCLPointCloud2::Ptr after_sample(new pcl::PCLPointCloud2());

	// Fill in the cloud data
	pcl::PCDReader reader;
	pcl::PCDWriter writer;

	/*
	reader.read<pcl::PointXYZ>("table_scene_lms400.pcd", *before);

	std::cerr << "Cloud before filtering: " << std::endl;
	std::cerr << *before << std::endl;


	// Create the filtering object
	pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
	sor.setInputCloud(before);
	sor.setMeanK(50);
	sor.setStddevMulThresh(1.0);
	sor.filter(*after);
	
	std::cerr << "Cloud after filtering: " << std::endl;
	std::cerr << *after << std::endl;
	

	writer.write<pcl::PointXYZ>("table_scene_lms400_inliers.pcd", *after, false);
	*/


	pcl::io::loadPCDFile("table_scene_lms400.pcd", *before);
	pcl::io::loadPCDFile("table_scene_lms400_inliers.pcd", *after);
	/*
	// Replace the path below with the path where you saved your file
	reader.read("table_scene_lms400_inliers.pcd", *before_sample); // Remember to download the file first!

	std::cerr << "PointCloud before filtering: " << before_sample->width * before_sample->height
		<< " data points (" << pcl::getFieldsList(*before_sample) << ").\n";

	// Create the filtering object
	pcl::VoxelGrid<pcl::PCLPointCloud2> sor1;
	sor1.setInputCloud(before_sample);
	sor1.setLeafSize(0.01f, 0.01f, 0.01f);
	sor1.filter(*after_sample);

	std::cerr << "PointCloud after filtering: " << after_sample->width * after_sample->height
		<< " data points (" << pcl::getFieldsList(*after_sample) << ")." << std::endl;


	writer.write("table_scene_lms400_downsampled.pcd", *after_sample,
		Eigen::Vector4f::Zero(), Eigen::Quaternionf::Identity(), false);

		*/


	pcl::io::loadPCDFile("table_scene_lms400_downsampled.pcd", *downsampleoutliers);


	while (count) {


		int a;
		std::cout << "숫자를 입력하세요 : ";
		std::cin >> a;
		pcl::visualization::CloudViewer viewer("Cloud Viewer");
		viewer.runOnVisualizationThreadOnce(viewerOneOff);

		switch (a)
		{

		case 0:

			viewer.showCloud(before);
			break;

		case 1:

			viewer.showCloud(after);
			
			break;
			
		case 2:

			viewer.showCloud(downsampleoutliers);
		
			break;
		default:

			count = false;
			return (0);
		}

		viewer.runOnVisualizationThread(viewerPsycho);
		while (!viewer.wasStopped())
		{
			user_data++;
		}
		
	}

}