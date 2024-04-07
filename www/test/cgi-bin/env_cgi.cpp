#include <iostream>

int main(int ac, char **av, char **env){
  std::cout << "Content-type: text/html; charset=utf-8\r\n\r\n";
  std::cout << "<html><head><title>サーバーテスト</title><meta http-equiv=\"content-type\" charset=\"utf-8\"><link rel=\"stylesheet\" href=\"/styles.css\"></head><body><header><nav><a href=\"/\">ホーム</a></nav></header><ul>";
  
  for (unsigned int i = 0; env[i] != NULL; i++){
    std::cout << "<li>" << env[i] << "</li>";
  }
  std::cout << "</ul></body></html>" << std::endl;
}
