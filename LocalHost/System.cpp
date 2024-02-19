#include "System.h"
#include <iostream>

/*
 * defining whether root directory will be c:\ or /
 * depending on OS
 */
#ifdef _WIN64
std::filesystem::directory_entry System::starting_point{ "C:\\" };
#elif _WIN32
std::filesystem::directory_entry System::starting_point{ "C:\\" };
#elif __linux__
std::filesystem::directory_entry System::starting_point{ "/" };
#endif


std::atomic<bool> System::file_is_found = false;
std::vector<std::thread> System::threads{};


std::filesystem::path System::find_file_path(const std::string& file_name, 
												const std::filesystem::directory_entry& start_path)
{
	/*
	 * function's logic:
	 *
	 * while file isn't found, traverse directory (excluding admin dirs),
	 * if file is found, change the flag to 'true' and tell other threads to stop searching.
	 * return file path.
	 */
	while (!System::file_is_found.load()) {
		for (const auto& entry : std::filesystem::recursive_directory_iterator(start_path,
									std::filesystem::directory_options::skip_permission_denied)) {
			try {
				if (entry.path().filename() == file_name) {
					std::cout << "found file here: " << entry.path() << '\n';
					System::file_is_found.store(true);
					return entry.path();
				}
			}
			/*
			 * if an exception is thrown, handle it
			 * by outputting the description to console.
			 */
			catch (std::exception& ex) {
				std::cerr << "Error: " << ex.what() << '\n';
				break;
			}
		}
	}
	// if nothing is found, return empty path.
	return {};
}


void System::thread_search(const std::string& file_name)
{
	/*
	 * function logic:
	 *
	 * search for all subdirectories in root directory and
	 * create a thread to search user's file in every one of them.
	 * if there is a file on root directory, check whether it
	 * is the one we search for. if it is, set flag file_is_found
	 * to true and exit. if it is not, continue searching.
	 */
	for (const std::filesystem::directory_entry& dir_entry :
		std::filesystem::directory_iterator(System::starting_point,
			std::filesystem::directory_options::skip_permission_denied)) {
		if (!System::file_is_found) {
			if (dir_entry.is_directory())
			{
				System::threads.emplace_back([file_name, dir_entry]()
					{ find_file_path(file_name, dir_entry); });
			}
			else if (dir_entry.is_regular_file() &&
				dir_entry.path().filename() == file_name)
			{
				std::cout << "file is directly on disk C. location: " << dir_entry.path() << '\n';
				System::file_is_found.store(true);
				break;
			}
		}
	}
}