#pragma once
#include <string>
#include <filesystem>
#include <thread>
#include <atomic>
#include <vector>

class System
{
public:
	// flag for threads' activity
	static std::atomic<bool> file_is_found;
	// function to find file using its name and subdirectory to search from
	static std::filesystem::path find_file_path(const std::string& file_name,
												const std::filesystem::directory_entry& start_path);
	// root directory, C:\ or /
	static std::filesystem::directory_entry starting_point;
	// vector for multithreading usage
	static std::vector<std::thread> threads;
	// function which creates threads that call find_file_path
	static void thread_search(const std::string& file_name);
};
