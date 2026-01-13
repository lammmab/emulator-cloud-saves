package server

import (
	"net/http"

	"github.com/gorilla/mux"
)

func NewServer() http.Handler {
	r := mux.NewRouter()
	r.HandleFunc("/ping", HandlePing).Methods("GET")
	r.HandleFunc("/upload", HandleUpload).Methods("POST")
	r.HandleFunc("/download/{save_id}", HandleDownload).Methods("GET")

	r.Use(CheckAuthorization)
	r.Use(CheckSize)

	return r
}
