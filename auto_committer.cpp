#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <chrono>

int main() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm local = *std::localtime(&now_time);

  std::ostringstream oss;
  oss << std::put_time(&local, "%Y-%m-%d %H:%M:%S");
  std::string cur_datetime = oss.str();

  std::system("git pull origin main");

  constexpr char LOG_FILENAME[] = "commit_log.txt";
  constexpr char BASE_MSG[] = "committed at private repo";

  std::ofstream ofs(LOG_FILENAME, std::ios::app);
  ofs << cur_datetime << "\n";
  ofs.close();

  std::string command = 
    "git add . && git commit -m \"" + std::string(BASE_MSG) + " (" + cur_datetime + ")\"";
      
  std::system(command.c_str());

  std::system("git push");
}