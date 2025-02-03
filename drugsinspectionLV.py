#!/usr/bin/env python
# coding: utf-8

import cv2
import easyocr
import pytesseract
from pyzbar import pyzbar

# Spécifier le chemin de Tesseract s'il n'est pas dans le PATH
pytesseract.pytesseract.tesseract_cmd = r'C:\Program Files\Tesseract-OCR\tesseract.exe'

# Fonction pour extraire le texte à partir d'une image
def extract_text(image):
    reader = easyocr.Reader(['fr'])  # Définir la langue de reconnaissance (par exemple, 'fr' pour le français)
    result = reader.readtext(image)
    extracted_text = ""
    for detection in result:
        text = detection[1]
        extracted_text += text + "\n"
    return extracted_text

# Fonction pour lire les codes à barres à partir d'une image
def read_barcodes(image):
    barcodes = pyzbar.decode(image)
    barcode_info = ""
    for barcode in barcodes:
        barcode_data = barcode.data.decode('utf-8')
        barcode_type = barcode.type
        barcode_info += f"Found {barcode_type} barcode: {barcode_data}\n"
    return barcode_info

# Fonction pour la reconnaissance de texte et de codes à barres en temps réel
def real_time_text_and_barcode_recognition():
    cap = cv2.VideoCapture(0)  # Accéder à la caméra du téléphone via IVcam (utilisez l'index de caméra approprié si nécessaire)
    
    while True:
        ret, frame = cap.read()  # Lire une frame de la caméra
        if not ret:
            break
        
        # Convertir la frame en RGB (easyocr nécessite des images au format RGB)
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        
        # Extraire automatiquement le texte
        text_extracted = extract_text(frame_rgb)
        
        # Lire les codes à barres
        barcodes_extracted = read_barcodes(frame)
        
        # Afficher le texte et les codes à barres extraits sur la vidéo
        cv2.putText(frame, text_extracted, (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)
        cv2.putText(frame, barcodes_extracted, (50, 100), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        
        # Afficher le texte et les codes à barres extraits dans le terminal
        print(text_extracted)
        print(barcodes_extracted)
        
        # Afficher la vidéo avec le texte et les codes à barres extraits
        cv2.imshow('Text and Barcode Extraction from Video', frame)

        # Attendre la pression de la touche 'q' pour quitter
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

# Exécution principale du code
if __name__ == "__main__":
    real_time_text_and_barcode_recognition()
