/**
 * @brief Copter visualization
 * @file copter_visualization.cpp
 * @author M.H.Kabir <mhkabir98@gmail.com>
 */
/*
 * Copyright 2014,2015 M.H.Kabir
 *
 * This file is part of the mavros package and subject to the license terms
 * in the top-level LICENSE file of the mavros repository.
 * https://github.com/mavlink/mavros/tree/master/LICENSE.md
 */
/*
 * @modify_by  Sajjad Rahnama
 */

#include <vector>

#include <tf/tf.h>

#include <ros/ros.h>
#include <ros/console.h>

#include <geometry_msgs/PoseStamped.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <std_msgs/String.h>

// parameters
static std::string fixed_frame_id;
static std::string child_frame_id;
static double marker_scale;
static int max_track_size = 1000;

// merker publishers
ros::Publisher track_marker_pub;
ros::Publisher vehicle_marker_pub;

boost::shared_ptr<visualization_msgs::MarkerArray> vehicle_marker;
ros::Publisher line_marker_pub;
ros::Subscriber shapes_sub;
ros::Publisher shapes_marker_pub;

bool traingle_detect = false;
bool square_detect = false;
bool circle_detect = false;
bool pentagon_detect = false;
bool star_detect = false;
bool heart_detect = false;

std::string TRIANGLE = "triangle";
std::string SQUARE = "square";
std::string CIRCLE = "circle";
std::string PENTAGON = "pentagon";
std::string STAR = "star";
std::string HEART = "heart";

void shapesCallback(const std_msgs::String &msg)
{
  std::string tmp(msg.data.c_str());
  if (tmp == TRIANGLE)
  {
    traingle_detect = true;
  }

  if (tmp == SQUARE)
  {
    square_detect = true;
  }

  if (tmp == CIRCLE)
  {
    circle_detect = true;
  }

  if (tmp == PENTAGON)
  {
    pentagon_detect = true;
  }

  if (tmp == STAR)
  {
    star_detect = true;
  }

  if (tmp == HEART)
  {
    heart_detect = true;
  }

}

/**
 * @brief publish vehicle track
 */
static void publish_track_marker(const geometry_msgs::PoseStamped::ConstPtr &pose)
{
  static boost::shared_ptr<visualization_msgs::Marker> track_marker;
  static boost::shared_ptr<visualization_msgs::Marker> line_marker;

  static boost::shared_ptr<visualization_msgs::Marker> traingle_marker;
  static boost::shared_ptr<visualization_msgs::Marker> square_marker;
  static boost::shared_ptr<visualization_msgs::Marker> circle_marker;
  static boost::shared_ptr<visualization_msgs::Marker> pentagon_marker;
  static boost::shared_ptr<visualization_msgs::Marker> star_marker;
  static boost::shared_ptr<visualization_msgs::Marker> heart_marker;

  if (!track_marker)
  {
    track_marker = boost::make_shared<visualization_msgs::Marker>();
    track_marker->type = visualization_msgs::Marker::CUBE_LIST;
    track_marker->ns = "fcu";
    track_marker->action = visualization_msgs::Marker::ADD;
    track_marker->scale.x = marker_scale * 0.015;
    track_marker->scale.y = marker_scale * 0.015;
    track_marker->scale.z = marker_scale * 0.015;
    track_marker->color.a = 1.0;
    track_marker->color.r = 0.0;
    track_marker->color.g = 0.0;
    track_marker->color.b = 0.5;
    track_marker->points.reserve(max_track_size);

    line_marker = boost::make_shared<visualization_msgs::Marker>();
    line_marker->ns = "fcu1";
    line_marker->action = visualization_msgs::Marker::ADD;
    line_marker->pose.orientation.w = 1.0;
    line_marker->type = visualization_msgs::Marker::LINE_STRIP;
    line_marker->scale.x = 0.4;
    line_marker->scale.y = 0.4;
    line_marker->scale.z = 0.4;
    line_marker->color.r = 1.0;
    line_marker->color.a = 1.0;

    //traingle
    traingle_marker = boost::make_shared<visualization_msgs::Marker>();
    traingle_marker->type = visualization_msgs::Marker::CUBE_LIST;
    traingle_marker->ns = "fcu2";
    traingle_marker->action = visualization_msgs::Marker::ADD;
    traingle_marker->scale.x = 0.5;
    traingle_marker->scale.y = 0.5;
    traingle_marker->scale.z = 0.5;
    traingle_marker->color.a = 1.0;
    traingle_marker->color.r = 1.0;
    traingle_marker->color.g = 0.5;
    traingle_marker->color.b = 0.5;

    //square
    square_marker = boost::make_shared<visualization_msgs::Marker>();
    square_marker->type = visualization_msgs::Marker::CUBE_LIST;
    square_marker->ns = "fcu3";
    square_marker->action = visualization_msgs::Marker::ADD;
    square_marker->scale.x = 0.5;
    square_marker->scale.y = 0.5;
    square_marker->scale.z = 0.5;
    square_marker->color.a = 1.0;
    square_marker->color.r = 0.3;
    square_marker->color.g = 0.2;
    square_marker->color.b = 0.1;

    //circle
    circle_marker = boost::make_shared<visualization_msgs::Marker>();
    circle_marker->type = visualization_msgs::Marker::CUBE_LIST;
    circle_marker->ns = "fcu4";
    circle_marker->action = visualization_msgs::Marker::ADD;
    circle_marker->scale.x = 0.5;
    circle_marker->scale.y = 0.5;
    circle_marker->scale.z = 0.5;
    circle_marker->color.a = 1.0;
    circle_marker->color.r = 0.1;
    circle_marker->color.g = 0.2;
    circle_marker->color.b = 0.3;

    //pentagon
    pentagon_marker = boost::make_shared<visualization_msgs::Marker>();
    pentagon_marker->type = visualization_msgs::Marker::CUBE_LIST;
    pentagon_marker->ns = "fcu5";
    pentagon_marker->action = visualization_msgs::Marker::ADD;
    pentagon_marker->scale.x = 0.5;
    pentagon_marker->scale.y = 0.5;
    pentagon_marker->scale.z = 0.5;
    pentagon_marker->color.a = 1.0;
    pentagon_marker->color.r = 0.6;
    pentagon_marker->color.g = 0.8;
    pentagon_marker->color.b = 0.9;

    //star
    star_marker = boost::make_shared<visualization_msgs::Marker>();
    star_marker->type = visualization_msgs::Marker::CUBE_LIST;
    star_marker->ns = "fcu6";
    star_marker->action = visualization_msgs::Marker::ADD;
    star_marker->scale.x = 0.5;
    star_marker->scale.y = 0.5;
    star_marker->scale.z = 0.5;
    star_marker->color.a = 1.0;
    star_marker->color.r = 0.7;
    star_marker->color.g = 0.6;
    star_marker->color.b = 0.8;

    //heart
    heart_marker = boost::make_shared<visualization_msgs::Marker>();
    heart_marker->type = visualization_msgs::Marker::CUBE_LIST;
    heart_marker->ns = "fcu7";
    heart_marker->action = visualization_msgs::Marker::ADD;
    heart_marker->scale.x = 0.5;
    heart_marker->scale.y = 0.5;
    heart_marker->scale.z = 0.5;
    heart_marker->color.a = 1.0;
    heart_marker->color.r = 0.8;
    heart_marker->color.g = 0.6;
    heart_marker->color.b = 0.8;
  }

  static int marker_idx = 0;

  if (track_marker->points.size() < max_track_size)
    track_marker->points.push_back(pose->pose.position);
  else track_marker->points[marker_idx] = pose->pose.position;

  marker_idx = ++marker_idx % max_track_size;

  track_marker->header = pose->header;
  track_marker_pub.publish(track_marker);

  line_marker->header = pose->header;
  line_marker->points.push_back(pose->pose.position);
  line_marker_pub.publish(line_marker);

  if (traingle_detect)
  {
    traingle_marker->header = pose->header;
    traingle_marker->points.push_back(pose->pose.position);
    shapes_marker_pub.publish(traingle_marker);
//    ROS_INFO("I heard: [%s]", "triangle");
  }

  if (square_detect)
  {
    square_marker->header = pose->header;
    square_marker->points.push_back(pose->pose.position);
    shapes_marker_pub.publish(square_marker);
//    ROS_INFO("I heard: [%s]", "square");
  }

  if (circle_detect)
  {
    circle_marker->header = pose->header;
    circle_marker->points.push_back(pose->pose.position);
    shapes_marker_pub.publish(circle_marker);
//    ROS_INFO("I heard: [%s]", "circle");
  }

  if (pentagon_detect)
  {
    pentagon_marker->header = pose->header;
    pentagon_marker->points.push_back(pose->pose.position);
    shapes_marker_pub.publish(pentagon_marker);
//    ROS_INFO("I heard: [%s]", "pentagon");
  }

  if (star_detect)
  {
    star_marker->header = pose->header;
    star_marker->points.push_back(pose->pose.position);
    shapes_marker_pub.publish(star_marker);
//    ROS_INFO("I heard: [%s]", "star");
  }

  if (heart_detect)
  {
    heart_marker->header = pose->header;
    heart_marker->points.push_back(pose->pose.position);
    shapes_marker_pub.publish(heart_marker);
//    ROS_INFO("I heard: [%s]", "heart");
  }

  traingle_detect = false;
  square_detect = false;
  circle_detect = false;
  pentagon_detect = false;
  star_detect = false;
  heart_detect = false;
}

/**
 * @brief publish vehicle
 */
static void create_vehicle_markers(int num_rotors, float arm_len, float body_width, float body_height)
{
  if (num_rotors <= 0) num_rotors = 2;

  /** Create markers only once for efficiency
   *  TODO use visualization_msgs::MarkerArray?
   */

  if (vehicle_marker)
    return;

  vehicle_marker = boost::make_shared<visualization_msgs::MarkerArray>();
  vehicle_marker->markers.reserve(2 * num_rotors + 1);

  /** Hexacopter marker code adapted from libsfly_viz
   *  thanks to Markus Achtelik.
   */

  // rotor marker template
  visualization_msgs::Marker rotor;
  rotor.header.stamp = ros::Time();
  rotor.header.frame_id = child_frame_id;
  rotor.ns = "vehicle_rotor";
  rotor.action = visualization_msgs::Marker::ADD;
  rotor.type = visualization_msgs::Marker::CYLINDER;
  rotor.scale.x = 0.2 * marker_scale;
  rotor.scale.y = 0.2 * marker_scale;
  rotor.scale.z = 0.01 * marker_scale;
  rotor.color.r = 0.4;
  rotor.color.g = 0.4;
  rotor.color.b = 0.4;
  rotor.color.a = 0.8;
  rotor.pose.position.z = 0;

  // arm marker template
  visualization_msgs::Marker arm;
  arm.header.stamp = ros::Time();
  arm.header.frame_id = child_frame_id;
  arm.ns = "vehicle_arm";
  arm.action = visualization_msgs::Marker::ADD;
  arm.type = visualization_msgs::Marker::CUBE;
  arm.scale.x = arm_len * marker_scale;
  arm.scale.y = 0.02 * marker_scale;
  arm.scale.z = 0.01 * marker_scale;
  arm.color.r = 0.0;
  arm.color.g = 0.0;
  arm.color.b = 1.0;
  arm.color.a = 1.0;
  arm.pose.position.z = -0.015 * marker_scale;

  float angle_increment = 2 * M_PI / num_rotors;

  for (float angle = angle_increment / 2; angle <= (2 * M_PI); angle += angle_increment)
  {
    rotor.pose.position.x = arm_len * cos(angle) * marker_scale;
    rotor.pose.position.y = arm_len * sin(angle) * marker_scale;
    rotor.id++;

    arm.pose.position.x = rotor.pose.position.x / 2;
    arm.pose.position.y = rotor.pose.position.y / 2;
    arm.pose.orientation = tf::createQuaternionMsgFromYaw(angle);
    arm.id++;

    vehicle_marker->markers.push_back(rotor);
    vehicle_marker->markers.push_back(arm);
  }

  // body marker template
  visualization_msgs::Marker body;
  body.header.stamp = ros::Time();
  body.header.frame_id = child_frame_id;
  body.ns = "vehicle_body";
  body.action = visualization_msgs::Marker::ADD;
  body.type = visualization_msgs::Marker::CUBE;
  body.scale.x = body_width * marker_scale;
  body.scale.y = body_width * marker_scale;
  body.scale.z = body_height * marker_scale;
  body.color.r = 0.0;
  body.color.g = 1.0;
  body.color.b = 0.0;
  body.color.a = 0.8;

  vehicle_marker->markers.push_back(body);

}

static void local_position_sub_cb(const geometry_msgs::PoseStamped::ConstPtr &pose)
{
  publish_track_marker(pose);
  if (vehicle_marker) vehicle_marker_pub.publish(vehicle_marker);
}

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "copter_visualization");
  ros::NodeHandle nh;
  ros::NodeHandle priv_nh("~");

  int num_rotors;
  double arm_len, body_width, body_height;

  priv_nh.param<std::string>("child_frame_id", child_frame_id, "copter_frame");

  priv_nh.param("marker_scale", marker_scale, 5.0);
  priv_nh.param("num_rotors", num_rotors, 6);
  priv_nh.param("arm_len", arm_len, 0.22);
  priv_nh.param("body_width", body_width, 0.15);
  priv_nh.param("body_height", body_height, 0.10);
  priv_nh.param("max_track_size", max_track_size, 1000);

  create_vehicle_markers(num_rotors, arm_len, body_width, body_height);

  track_marker_pub = nh.advertise<visualization_msgs::Marker>("track_markers", 10);
  vehicle_marker_pub = nh.advertise<visualization_msgs::MarkerArray>("vehicle_marker", 10);
  line_marker_pub = nh.advertise<visualization_msgs::Marker>("line_marker", 10);
  shapes_marker_pub = nh.advertise<visualization_msgs::Marker>("shapes_marker", 10);
  auto pos_sub = nh.subscribe("/mavros/local_position/pose", 10, local_position_sub_cb);
  shapes_sub = nh.subscribe("shapes", 10, shapesCallback);

  ros::spin();
  return 0;
}

