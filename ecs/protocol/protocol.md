# Emulator Cloud Saves Protocol

## Auth
- Header: X-ECS-Auth: <token>
- Optional timestamp HMAC for verification

## /upload
- multipart/form-data:
  - save_id: string (TITLE ID)
  - file: binary zip
- Max file size: 128 MB
- Response: JSON { "success": true }

## /download/:save_id
- GET request with auth
- Returns binary zip
