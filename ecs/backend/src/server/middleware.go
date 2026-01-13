package server

import "net/http"

const MAX_UPLOAD_SIZE = 128 << 20

func CheckAuthorization(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		token := r.Header.Get("X-ECS-Auth")
		if !ValidateToken(token) {
			http.Error(w, "Not Authorized", http.StatusUnauthorized)
			return
		}
		next.ServeHTTP(w, r)
	})
}

func CheckSize(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		if r.Method == "POST" && r.URL.Path == "/upload" {
			r.Body = http.MaxBytesReader(w, r.Body, MAX_UPLOAD_SIZE)
			if err := r.ParseMultipartForm(MAX_UPLOAD_SIZE); err != nil {
				http.Error(w, "File too large", http.StatusRequestEntityTooLarge)
				return
			}
		}
		next.ServeHTTP(w, r)
	})
}
