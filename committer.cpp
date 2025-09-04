#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <chrono>
#include <thread>

struct Date {
  short year, month, day;
};

struct Time {
  short hour, minute, second;
};

std::mt19937 random_generator(std::random_device{}());
std::uniform_int_distribution<int> commitCounts(0, 5);
std::uniform_int_distribution<int> hourDist(0, 23);
std::uniform_int_distribution<int> minDist(0, 59);
std::uniform_int_distribution<int> secDist(0, 59);

constexpr char LOG_FILENAME[] = "commit_log.txt";
constexpr char BASE_MSG[] = "committed at private repo";


std::string formatDate(const Date& commit_date, const Time& commit_time) {
  std::ostringstream oss;
  oss << commit_date.year << "-"
      << (commit_date.month < 10 ? "0" : "") << commit_date.month << "-"
      << (commit_date.day < 10 ? "0" : "") << commit_date.day << " "
      << (commit_time.hour < 10 ? "0" : "") << commit_time.hour << ":"
      << (commit_time.minute < 10 ? "0" : "") << commit_time.minute << ":"
      << (commit_time.second < 10 ? "0" : "") << commit_time.second;
    return oss.str();
}

void randomizeTime(Time& time) {
  time.hour = hourDist(random_generator);
  time.minute = minDist(random_generator);
  time.second = secDist(random_generator);
}

void writeToFile(const std::string& formattedDate) {
  std::ofstream ofs(LOG_FILENAME, std::ios::app);
  ofs << formattedDate << "\n";
  ofs.close();
}

int main() {
  std::system("git pull origin main");
  Date commit_date;
  Time commit_time;
  
  int t;
  std::cin >> t;
  while(t--) {
    std::cin >> commit_date.year >> commit_date.month >> commit_date.day;
    for (int i = commitCounts(random_generator); i > 0; --i) {
      randomizeTime(commit_time);
      std::string formattedDate = formatDate(commit_date, commit_time);

      writeToFile(formattedDate);
      
      std::string command = 
        "git add . && set GIT_COMMITTER_DATE=\"" + formattedDate + "\" && git commit --date=\"" + formattedDate + 
        "\" -am \"" + std::string(BASE_MSG) + " (" + formattedDate + ")\"";
      
      std::system(command.c_str());
      std::cout << "Committed at " << formattedDate << "\n";
    }
    std::system("git push origin main");
    std::this_thread::sleep_for(std::chrono::seconds(120));
  }
}