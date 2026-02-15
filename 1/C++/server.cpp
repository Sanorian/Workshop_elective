#include <crow.h>
#include <string>
#include <optional>
#include <vector>

// "Модель" товара (аналог Pydantic)
struct Item {
    std::string name;
    std::optional<std::string> description;
    double price;
    bool is_available;

    // Сериализация в JSON
    crow::json::wvalue to_json() const {
        crow::json::wvalue json;
        json["name"] = name;
        if (description.has_value()) {
            json["description"] = description.value();
        }
        json["price"] = price;
        json["is_available"] = is_available;
        return json;
    }

    // Десериализация из JSON
    static Item from_json(const crow::json::rvalue& json) {
        Item item;
        item.name = json["name"].s();
        if (json.has("description")) {
            item.description = json["description"].s();
        }
        item.price = json["price"].d();
        item.is_available = json["is_available"].b();
        return item;
    }
};

int main() {
    crow::SimpleApp app;

    // CORS middleware (для работы с браузером)
    CROW_ROUTE(app, "/")
        .methods("GET"_method)
        ([](){
            crow::json::wvalue response;
            response["message"] = "Добро пожаловать на мой C++ Crow сервер!";
            return crow::response{response};
        });

    // GET /items/{item_id}
    CROW_ROUTE(app, "/items/<int>")
        .methods("GET"_method)
        ([](int item_id){
            crow::json::wvalue response;
            response["item_id"] = item_id;
            return crow::response{response};
        });

    // GET /items/ с query параметрами
    CROW_ROUTE(app, "/items/")
        .methods("GET"_method)
        ([](const crow::request& req){
            int skip = 0, limit = 10;
            
            if (req.url_params.get("skip")) {
                skip = std::stoi(req.url_params.get("skip"));
            }
            if (req.url_params.get("limit")) {
                limit = std::stoi(req.url_params.get("limit"));
            }
            
            crow::json::wvalue response;
            response["skip"] = skip;
            response["limit"] = limit;
            return crow::response{response};
        });

    // POST /items/
    CROW_ROUTE(app, "/items/")
        .methods("POST"_method)
        ([](const crow::request& req){
            auto json = crow::json::load(req.body);
            if (!json) {
                return crow::response(400, "Invalid JSON");
            }
            
            try {
                Item item = Item::from_json(json);
                
                crow::json::wvalue response;
                response["message"] = "Элемент успешно создан";
                response["item"] = item.to_json();
                
                return crow::response{response};
            } catch (const std::exception& e) {
                return crow::response(400, "Error parsing item");
            }
        });

    // PUT /items/{item_id}
    CROW_ROUTE(app, "/items/<int>")
        .methods("PUT"_method)
        ([](const crow::request& req, int item_id){
            auto json = crow::json::load(req.body);
            if (!json) {
                return crow::response(400, "Invalid JSON");
            }
            
            try {
                Item item = Item::from_json(json);
                
                crow::json::wvalue response;
                response["item_id"] = item_id;
                response["updated_item"] = item.to_json();
                
                return crow::response{response};
            } catch (const std::exception& e) {
                return crow::response(400, "Error parsing item");
            }
        });

    // DELETE /items/{item_id}
    CROW_ROUTE(app, "/items/<int>")
        .methods("DELETE"_method)
        ([](int item_id){
            crow::json::wvalue response;
            response["message"] = "Элемент " + std::to_string(item_id) + " удален";
            return crow::response{response};
        });

    // GET /users/{user_id}/items/{item_id}
    CROW_ROUTE(app, "/users/<int>/items/<int>")
        .methods("GET"_method)
        ([](const crow::request& req, int user_id, int item_id){
            crow::json::wvalue response;
            response["user_id"] = user_id;
            response["item_id"] = item_id;
            
            if (req.url_params.get("q")) {
                response["q"] = req.url_params.get("q");
            }
            
            return crow::response{response};
        });

    // POST /echo/
    CROW_ROUTE(app, "/echo/")
        .methods("POST"_method)
        ([](const crow::request& req){
            auto json = crow::json::load(req.body);
            if (!json) {
                return crow::response(400, "Invalid JSON");
            }
            
            crow::json::wvalue response;
            response["received"] = json;
            
            return crow::response{response};
        });

    // Запуск сервера
    app.port(8080).multithreaded().run();
    
    return 0;
}