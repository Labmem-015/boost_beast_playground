"""
Модуль для создания базового HTTP-сервера с использованием FastAPI.
Предназначен для демонстрации приёма соединений и обработки содержимого запроса.
"""

from fastapi import FastAPI, Request, HTTPException
import json
import logging
from datetime import datetime
from typing import Mapping

# Настройка логгирования
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s',
    datefmt='%Y-%m-%d %H:%M:%S'
)
logger = logging.getLogger(__name__)

# Инициализация приложения FastAPI
app = FastAPI(
    title="FastAPI Server",
    description="Сервер для приёма и анализа HTTP-запросов",
    version="1.0.0"
)


def log_connection_info(request: Request):
    """Логирование информации о подключении в консоль."""
    logger.info(f"\n{'='*60}")
    logger.info(f"Новое соединение:")
    logger.info(f"  Метод: {request.method}")
    logger.info(f"  URL: {request.url}")
    logger.info(f"  Клиент: {request.client.host if request.client else 'Неизвестно'}")
    logger.info(f"  Время: {datetime.now().isoformat()}")
    logger.info(f"\n{'─'*60}")

    # Логирование заголовков
    logger.info(f"Заголовки запроса ({len(dict(request.headers))} шт.):")
    headers: Mapping[str, str] = request.headers
    for key, value in headers.items():
        max_length = 200
        if len(value) > max_length:
            display_value = value[:max_length] + "..."
        else:
            display_value = value

        logger.info(f"  {key}: {display_value}")
    logger.info(f"{'─'*60}\n")


@app.get("/")
async def root(request: Request):
    """Эндпоинт проверки доступности сервера."""
    log_connection_info(request)
    return {"status": "ok", "message": "Сервер запущен успешно"}


@app.post("/echo")
async def echo_endpoint(request: Request):
    """Эндпоинт для получения полной информации о POST-запросе."""
    log_connection_info(request)

    try:
        body_content = await request.body()
        content_type = request.headers.get("content-type", "").lower()

        # Декодируем тело с обработкой ошибок кодировки
        body_str = body_content.decode("utf-8", errors="replace")

        if "application/json" in content_type:
            try:
                body_json = json.loads(body_str)
            except json.JSONDecodeError:
                body_json = body_str  # если не JSON, возвращаем как строку
        elif "multipart/form-data" in content_type:
            form = await request.form()
            body_json = {}
            for key, value in form.items():
                # Проверяем, является ли значение файлом (UploadFile)
                if hasattr(value, "filename"):
                    body_json[key] = {
                        "filename": value.filename,
                        "content_type": value.content_type,
                        "size": value.size  # может быть None
                    }
                else:
                    body_json[key] = str(value)
        else:
            # Для остальных типов возвращаем сырые данные как строку
            body_json = body_str

        response_data = {
            "status": "success",
            "method": request.method,
            "url_path": str(request.url),
            "headers": dict(request.headers),
            "body": body_json,
            "client_host": request.client.host if request.client else None
        }

        return response_data

    except Exception as exc:
        logger.error(f"Ошибка обработки запроса: {exc}", exc_info=True)
        # Возвращаем клиенту понятную ошибку, не раскрывая внутренности
        raise HTTPException(status_code=500, detail="Internal server error")


if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000, timeout_keep_alive=5)