package server

import (
	"fmt"
	"net/http"

	"github.com/lammmab/emulator-cloud-saves/src/storage"
)

func HandlePing(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusOK)
	w.Write([]byte(`{"status":"ok"}`))
}

func HandleUpload(w http.ResponseWriter, r *http.Request) {
	file, header, err := r.FormFile("file")

	if err != nil {
		http.Error(w, "Failed to read uploaded file", http.StatusBadRequest)
		return
	}
	defer file.Close()

	userID := r.Context().Value("user_id").(string)

	path, err := storage.SaveFile(userID, header.Filename, file)
	if err != nil {
		http.Error(w, "Failed to save file", http.StatusInternalServerError)
		return
	}

	fmt.Println("Saved file at:", path)
	w.WriteHeader(http.StatusOK)
}

func HandleDownload(w http.ResponseWriter, r *http.Request) {

}

func HandleAuthentication(w http.ResponseWriter, r *http.Request) {

}
