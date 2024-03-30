#include "ofstream"
#include "iostream"

int main() {
  std::ofstream ofs("a.txt");
  if (!ofs) {
    return Some(GenerateErrorResponse(http::kInternalServerError, config));
  }
}
