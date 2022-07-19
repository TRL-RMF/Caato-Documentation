# Deployment & Operation Concerns 

# 3D Mapping

# 3D Mapping Notes

## 1. Initial Mapping

In order to start mapping, we need to run the mapping launch file and simultaneously record rosbag

Example of such mapping launch file:

```jsx
<launch>
    <!-- <include file="$(find caato2_bringup)/launch/include/description_caato_3d.launch" /> -->

    <include file="$(find caato2_bringup)/launch/include/caato2_3d_lasers.launch" />

    <include file="$(find roboteq_diff_driver)/launch/driver.launch" >
    <arg name="base_frame" value="base_link"/>
    <arg name="pub_odom_tf" value="true"/>
    </include>

    <include file="$(find caato_teleop)/launch/ds4.launch" />

</launch>
```

After running the launch file, run `rosbag record -a` in another terminal.

After completing the mapping, kill the rosbag and followed by the launch file.

## 2. Obtaining the map

In this stage make use of offline cart node in google cartographer and pass the bag as an argument

The command would be:

```jsx
roslaunch cartographer_ros offline_cart_3d.launch bag_filenames:="</path_to_bag>"
```

After the node completes cleanly, a .pbstream file will be created.

offline_cart_node.launch:

```jsx
<launch>
  <param name="/use_sim_time" value="true" />

  <node name="rviz" pkg="rviz" type="rviz" required="true"
      args="-d $(find cartographer_ros)/configuration_files/demo_3d.rviz" />

  <node name="cartographer_offline_node" pkg="cartographer_ros"
      type="cartographer_offline_node" args="
          -configuration_directory $(find cartographer_ros)/configuration_files
          -configuration_basenames cart_3d.lua
          -urdf_filenames $(find cartographer_ros)/urdf/os1_sensor.urdf
          -bag_filenames $(arg bag_filenames)"
      output="screen">
    <remap from="points2" to="/os_cloud_node/points" />
    <remap from="imu" to="/os_cloud_node/imu" />
  </node>

  <node name="cartographer_occupancy_grid_node" pkg="cartographer_ros"
      type="cartographer_occupancy_grid_node" args="-resolution 0.05" />
</launch>
```

Remember you need to pass .lua configuration and the lidar sensor urdf files in the offline cart launch. The example of those files can be seen below:

cart_3d.lua:

```jsx

include "map_builder.lua"
include "trajectory_builder.lua"

options = {
  map_builder = MAP_BUILDER,
  trajectory_builder = TRAJECTORY_BUILDER,
  map_frame = "map",
  tracking_frame = "os_imu",
  published_frame = "base_link",
  odom_frame = "base_link",
  provide_odom_frame = false,
  publish_frame_projected_to_2d = false,
  use_odometry = false,
  use_nav_sat = false,
  use_landmarks = false,
  num_laser_scans = 0,
  num_multi_echo_laser_scans = 0,
  num_subdivisions_per_laser_scan = 1,
  num_point_clouds = 1,
  lookup_transform_timeout_sec = 0.2,
  submap_publish_period_sec = 0.3,
  pose_publish_period_sec = 5e-3,
  trajectory_publish_period_sec = 30e-3,
  rangefinder_sampling_ratio = 1.,
  odometry_sampling_ratio = 1.,
  fixed_frame_pose_sampling_ratio = 1.,
  imu_sampling_ratio = 1.,
  landmarks_sampling_ratio = 1.,
}
-- Original defaults
TRAJECTORY_BUILDER_3D.num_accumulated_range_data = 1

MAP_BUILDER.use_trajectory_builder_3d = true
MAP_BUILDER.num_background_threads = 4

POSE_GRAPH.optimization_problem.huber_scale = 5e2
POSE_GRAPH.optimize_every_n_nodes = 320
POSE_GRAPH.constraint_builder.sampling_ratio = 0.03
POSE_GRAPH.optimization_problem.ceres_solver_options.max_num_iterations = 500
POSE_GRAPH.constraint_builder.min_score = 0.62
POSE_GRAPH.constraint_builder.global_localization_min_score = 0.66

-- Modifications
TRAJECTORY_BUILDER_3D.ceres_scan_matcher.translation_weight = 5.

TRAJECTORY_BUILDER_3D.use_online_correlative_scan_matching = true
TRAJECTORY_BUILDER_3D.imu_gravity_time_constant = .1

-- No point of trying to SLAM over the points on your cart.
TRAJECTORY_BUILDER_3D.min_range = 1.0
TRAJECTORY_BUILDER_3D.max_range = 50

-- These were just my first guess: use more points for SLAMing and adapt a bit for the ranges that are bigger for cars.
TRAJECTORY_BUILDER_3D.high_resolution_adaptive_voxel_filter.max_length = 5.
TRAJECTORY_BUILDER_3D.high_resolution_adaptive_voxel_filter.min_num_points = 250.
TRAJECTORY_BUILDER_3D.low_resolution_adaptive_voxel_filter.max_length = 8.
TRAJECTORY_BUILDER_3D.low_resolution_adaptive_voxel_filter.min_num_points = 400.

-- The submaps felt pretty big - since the car moves faster, we want them to be
-- slightly smaller. You are also slamming at 10cm - which might be aggressive
-- for cars and for the quality of the laser. I increased 'high_resolution',
-- you might need to increase 'low_resolution'. Increasing the
-- '*num_iterations' in the various optimization problems also trades
-- performance/quality.
TRAJECTORY_BUILDER_3D.submaps.high_resolution = .25
TRAJECTORY_BUILDER_3D.submaps.low_resolution = .60
TRAJECTORY_BUILDER_3D.submaps.num_range_data = 270
TRAJECTORY_BUILDER_3D.ceres_scan_matcher.ceres_solver_options.max_num_iterations = 50

-- Trying loop closing too often will cost CPU and not buy you a lot. There is
-- little point in trying more than once per submap.
MAP_BUILDER.pose_graph.optimize_every_n_nodes = 100
MAP_BUILDER.pose_graph.constraint_builder.sampling_ratio = 0.03
MAP_BUILDER.pose_graph.optimization_problem.ceres_solver_options.max_num_iterations = 200

MAP_BUILDER.pose_graph.constraint_builder.min_score = 0.5

-- Crazy search window to force loop closure to work. All other changes are probably not needed.
--MAP_BUILDER.sparse_pose_graph.constraint_builder.max_constraint_distance = 250.
--MAP_BUILDER.sparse_pose_graph.constraint_builder.fast_correlative_scan_matcher_3d.linear_xy_search_window = 250.
--MAP_BUILDER.sparse_pose_graph.constraint_builder.fast_correlative_scan_matcher_3d.linear_z_search_window = 30.
--MAP_BUILDER.sparse_pose_graph.constraint_builder.fast_correlative_scan_matcher_3d.angular_search_window = math.rad(60.)
--MAP_BUILDER.sparse_pose_graph.constraint_builder.ceres_scan_matcher_3d.ceres_solver_options.max_num_iterations = 50
```

os1_sensor.urdf

```jsx
<robot name="os_sensor">
  <material name="orange">
    <color rgba="1.0 0.5 0.2 1" />
  </material>
  <material name="gray">
    <color rgba="0.2 0.2 0.2 1" />
  </material>

  <link name="os_imu">
    <visual>
      <origin xyz="0.0 0.0 0.0" />
      <geometry>
        <box size="0.06 0.04 0.02" />
      </geometry>
      <material name="orange" />
    </visual>
  </link>

  <link name="os_lidar">
    <visual>
      <origin xyz="0.0 0.0 0.0" />
      <geometry>
        <cylinder length="0.07" radius="0.05" />
      </geometry>
      <material name="gray" />
    </visual>
  </link>

  <link name="os_sensor" />

  <link name="base_link" />

  <!-- Legacy data -->
  <link name="os" />

  <joint name="os_link_joint_legacy" type="fixed">
    <parent link="base_link" />
    <child link="os" />
    <origin xyz="0.00 0. 0.03618" rpy="0. 0 0" />
  </joint>

  <joint name="sensor_link_joint" type="fixed">
    <parent link="base_link" />
    <child link="os_sensor" />
    <origin xyz="0 0 0" rpy="0 0 0" />
  </joint>

  <joint name="imu_link_joint" type="fixed">
    <parent link="os_sensor" />
    <child link="os_imu" />
    <origin xyz="0.006253 -0.011775 0.007645" rpy="0 0 0" />
  </joint>

  <joint name="os_link_joint" type="fixed">
    <parent link="os_sensor" />
    <child link="os_lidar" />
    <origin xyz="0.0 0.0 0.03618" rpy="0 0 3.14159" />
  </joint>

</robot>
```

## 3. Obtain PCD file

In order to obtain the PCD file, we make use of cartographer’s build in node called assets_writer_cart_3d.

The command is as follows:

```jsx
roslaunch cartographer_ros assets_writer_cart_3d.launch
****bag_filenames:=${HOME}/Downloads/b3-2016-04-05-14-14-00.bag pose_graph_filename:=${HOME}/Downloads/b3-2016-04-05-14-14-00.bag.pbstream
```

This will be create a PCD file.

After obtaining the initial PCD file, it has to be down sampled in order to reduce the time it takes to create the .bt file in the next step.

The command is as follows:

```jsx
pcl_voxel_grid <input>.pcd downsampled.pcd -leaf 0.05,0.05,0.05
```

A new down sampled .pcd file will be created.

## 4. Obtain BT file

To obtain BT file, we have to make use of octomap tracking server and pcdTopointcloud 

The command for pcdTopointcloud launch is as follows:

```jsx
roslaunch octomap_server pcdTopointcloud.launch
```

Remember to change the file name in the pcdTopointcloud.launch and build again

The command is as follows:

```jsx
roslaunch octomap_server octomap_tracking_server.launch
```

Followed by octomap_saver

```jsx
rosrun octomap_server octomap_saver -f <name_of_map>.bt octomap_full:=octomap_full
```

This will produce a .BT file

Octomap_tracking_server.launch sample code:

```jsx
<launch>

  <arg name="path" default=""/>
  <arg name="changeIdFrame" default="/talker/changes"/>

    <!-- you can load an exisiting tree with <node ... args="tree.bt"> !-->
    <node pkg="octomap_server" type="octomap_tracking_server_node" name="octomap_talker"    output="screen" args="$(arg path)">
        <param name="resolution" value="0.05" />
        <param name="frame_id" type="string" value="map" />
        <param name="sensor_model/max_range" value="20.0" />
        <param name="save_directory" value="$(optenv OCTOMAP_SAVE_DIR ./)" />
        <param name="track_changes" value="true"/>
        <param name="listen_changes" value="false"/>
        <param name="topic_changes" value="/octomap_tracking_server/changeset" />
            <param name="change_id_frame" value="$(arg changeIdFrame)" />
            <param name="min_change_pub" value="0" />
            <param name="filter_ground" value="false" />
            <param name="pointcloud_max_z" value="0.5" />
            <param name="pointcloud_min_z" value="-0.7" />
            <param name="base_frame_id" value="base_link" />
        <!--remap from="cloud_in" to="/rgbdslam/batch_clouds" /-->
    </node>
</launch>
```

The most important arguments are:

1. pointcloud_max_z
2. pointcloud_min_z
3. base_frame_id

## 5. Obtaining 2D map

This step is similar to step 4, but with changes to pointcloud_max_z and pointcloud_min_z in the tracking_server launch file. The changes are as follows

1. pointcloud_max_z = 0.5
2. pointcloud_min_z = -0.5

Then rerun the following command:

```jsx
roslaunch octomap_server octomap_tracking_server.launch
```

🌟In this step, we will be making sure of the normal 2D map_saver instead of the octomap_server. Furthermore, change the topic that the server subscribes to from /map to /projected_map

Use the following command:

```jsx
rosrun map_server map_saver map:=/projected_map -f name_of_map
```

Map.yaml and map.png files will be created.