#ifndef SERVER_DATA
#define SERVER_DATA
#include <unordered_map>
#include <tuple>
class ServerData {
public:
    static ServerData& instance() {
        static ServerData instance; // 정적 지역 변수 (C++11 이상에서 thread-safe)
        return instance;
    }
    std::unordered_map<std::string, std::tuple<std::string, std::string>> user_map; // ' user_map[chat_id] = 클라이언트의 대화명, 접속중인 채팅방명 '
    std::vector<Glib::ustring> chat_list;                                           // 현존하는 채팅방명 목록
private:
    ServerData() {}                                       // 생성자: 외부에서 객체 생성을 차단
    ~ServerData() {}                                      // 소멸자
    ServerData(const ServerData&) = delete;             // 복사 생성 금지
    ServerData& operator=(const ServerData&) = delete;  // 대입 금지
};
extern ServerData& serverData;
#endif // SERVER_DATA