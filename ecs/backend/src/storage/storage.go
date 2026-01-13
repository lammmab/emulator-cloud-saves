package storage

import (
	"io"
	"os"
	"path/filepath"
)

var BASE_FOLDER = "./cloud_saves"

func SaveFile(id string, name string, file io.Reader) (string, error) {
	userFolder := filepath.Join(BASE_FOLDER, id)
	os.MkdirAll(userFolder, 0755)

	path := filepath.Join(userFolder, name)
	out, err := os.Create(path)
	if err != nil {
		return "", err
	}
	defer out.Close()

	_, err = io.Copy(out, file)
	if err != nil {
		return "", err
	}
	return path, nil
}
