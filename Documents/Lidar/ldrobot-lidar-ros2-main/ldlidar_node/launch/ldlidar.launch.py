#!/usr/bin/env python3

import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, Command
from launch_ros.actions import Node, LifecycleNode


def generate_launch_description():

    # Set LOG format
    os.environ['RCUTILS_CONSOLE_OUTPUT_FORMAT'] = '{time} [{severity}] ({name}) {message}'
    os.environ['RCUTILS_COLORIZED_OUTPUT'] = '1'

    node_name = LaunchConfiguration('node_name')

    # Lidar node configuration file
    lidar_config_path = os.path.join(
        get_package_share_directory('ldlidar_node'),
        'config',
        'ldlidar.yaml'
    )

    # Launch arguments
    declare_node_name_cmd = DeclareLaunchArgument(
        'node_name',
        default_value='ldlidar_node',
        description='Name of the node'
    )

    # LDLidar lifecycle node
    ldlidar_node = LifecycleNode(        
        package = 'ldlidar_node',
        executable = 'ldlidar_node',
        name = node_name,
        output='screen',
        parameters=[
            # YAML files
            lidar_config_path  # Parameters
        ]
    )

    # URDF path
    urdf_file_name = 'ldlidar_descr.urdf.xml'
    urdf = os.path.join(
        get_package_share_directory('ldlidar_node'),
        'urdf',
        urdf_file_name)
    with open(urdf, 'r') as infp:
        robot_desc = infp.read()

    # Robot State Publisher node
    rsp_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='ldlidar_state_publisher',
        output='screen',
        parameters=[{'robot_description': robot_desc}],
        arguments=[urdf]
    )

    # Define LaunchDescription variable
    ld = LaunchDescription()

    # Launch arguments
    ld.add_action(declare_node_name_cmd)

    # Launch Nav2 Lifecycle Manager
    ld.add_action(rsp_node)

    # LDLidar Lifecycle node
    ld.add_action(ldlidar_node)

    return ld
    