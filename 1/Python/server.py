from fastapi import FastAPI
from pydantic import BaseModel
from typing import Optional
import uvicorn

app = FastAPI()

# прописываем сущность "Товар"
class Item(BaseModel):
    name: str
    description: Optional[str] = None
    price: float
    is_available: bool = True

# GET-запрос на приветственный экран
@app.get("/")
async def root():
    return {"message": "Добро пожаловать на мой FastAPI сервер!"}

# GET-запросы с использованием параметра в url
@app.get("/items/{item_id}")
async def read_item(item_id: int):
    return {"item_id": item_id}

@app.get("/items/")
async def list_items(skip: int = 0, limit: int = 10):
    return {"skip": skip, "limit": limit}

# POST-запрос с типо-созданием нового товара
@app.post("/items/")
async def create_item(item: Item):
    return {
        "message": "Элемент успешно создан",
        "item": item
    }

# PUT-запрос с типо-обновлением данных товара
@app.put("/items/{item_id}")
async def update_item(item_id: int, item: Item):
    return {
        "item_id": item_id,
        "updated_item": item
    }

# DELETE-запрос с типо-удалением товара
@app.delete("/items/{item_id}")
async def delete_item(item_id: int):
    return {"message": f"Элемент {item_id} удален"}

# GET-запрос с типо-получением товаров конкретного пользователя
@app.get("/users/{user_id}/items/{item_id}")
async def get_user_item(user_id: int, item_id: int, q: Optional[str] = None):
    result = {"user_id": user_id, "item_id": item_id}
    if q:
        result["q"] = q
    return result

# POST-запрос, возвращающий те же данные, что и принял
@app.post("/echo/")
async def echo(data: dict):
    return {"received": data}

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8080)