---
title: Fast API Study Summary 
date: 2023-09-17 21:15:20 +0900
categories: [Server]
tags: [python, fastapi]		# tags MUST be small letters, like tag NOT TAG 
---

# FastAPI

**Notion AI Server Client**

1. Model Schema
2. Client Server
3. Pydantic Pedantic for enforcing rules similar to JS and TS

FastAPI and Uvicorn

Creating an API query structure is simple. Import Pydantic and declare it within a class like this:

```python
app = FastAPI()
```
Then, use the following format for API endpoints:

```python
@app.post("/items/")
async def create_item(item: Item):
    return item

```

**Use "Put" for Update and "Post" for Create!**

**Function Parameters**

- If declared within the path, it's interpreted as a path parameter.
- If it's a single type (int, str, bool, etc.), it's interpreted as a Query Parameter.
- If it's a Pydantic Model, it's interpreted as a Request Body.
- FastAPI Query is in JSON format but can be seen as a string.

```python
q: Union[str, None] = Query(default=None)
q: str | None = Query(default=None)
q: str | None = None
```

**Note:** In annotated types, you CANNOT use a default value.

**Python function annotation** helps IDEs recognize and understand variables explicitly specified, preventing issues when dealing with dynamic structures or objects.

**In a DB project**, the HTTP methods are typically used as follows:

- `GET`: Retrieve resources (often referred to as "Representation" nowadays).
- `POST`: Process request data, primarily used for registration.
- `PUT`: Replace a resource; if it doesn't exist, create it.
- `PATCH`: Partially modify a resource.
- `DELETE`: Delete a resource.
- `HEAD`: Similar to GET but only returns the status line and headers, excluding the message body.
- `OPTIONS`: Describe available communication options (methods) for a target resource (commonly used for CORS).
- `CONNECT`: Establish a tunnel to a server identified by a target resource.
- `TRACE`: Perform a message loopback test along the path to the target resource.

**Important:** Never use uppercase letters in URL routes. Uppercase letters in filenames are acceptable, but not in URLs.
