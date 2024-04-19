import threading
import cv2
from deepface import DeepFace
import os

# Chemin absolu du dossier contenant les images de référence
dossier_base = "C:/Users/user/Desktop/FACE RECOGNITION/"

# Liste des dossiers contenant les images de référence
dossiers_references = [
    "TRESORIER",
    "RESPONSABLE RH",
    "RESPONSABLE LOGISTIQUE",
    "RESPONSABLE SPONSORING",
    "RESPONSABLE PROGRAMMES",
    "RESPONSABLE INVITES"
]

# Charger les images de référence depuis chaque dossier
reference_images = {}
for dossier in dossiers_references:
    dossier_path = os.path.join(dossier_base, dossier)  # Chemin absolu du dossier
    reference_images[dossier] = []
    for filename in os.listdir(dossier_path):
        if filename.endswith(".jpg") or filename.endswith(".png"):
            img_path = os.path.join(dossier_path, filename)
            reference_images[dossier].append(cv2.imread(img_path))

cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

counter = 0
match_dossier = None
found_match = False  # Variable pour contrôler la sortie de la boucle

def check_face(frame):
    global match_dossier, found_match
    try:
        for dossier, images in reference_images.items():
            for reference_image in images:
                result = DeepFace.verify(img1_path=frame, img2_path=reference_image, enforce_detection=False)
                if result["verified"]:
                    match_dossier = dossier
                    found_match = True  # Mettre à jour la variable lorsque un match est trouvé
                    return
    except Exception as e:
        print(e)
        match_dossier = None

def get_result_code(dossier):
    codes = {
        "RESPONSABLE RH": 1,
        "RESPONSABLE INVITES": 2,
        "RESPONSABLE LOGISTIQUE": 3,
        "RESPONSABLE SPONSORING": 6,
        "RESPONSABLE PROGRAMMES": 4,
        "TRESORIER": 5
    }
    return codes.get(dossier, 0)  # Retourne 0 si le dossier n'est pas trouvé

while True:
    ret, frame = cap.read()
    if ret:
        counter += 1
        if counter % 30 == 0:
            threading.Thread(target=check_face, args=(frame.copy(),)).start()

        if match_dossier:
            cv2.putText(frame, f"MATCH: {match_dossier}", (20, 450), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
            if found_match:  # Vérifier si un match a été trouvé
                result_code = get_result_code(match_dossier)
                cv2.imshow("FACE RECOGNITION", frame)
                cv2.waitKey(2000)  # Afficher le résultat pendant 2 secondes
                print(f"Returning code: {result_code}")
                break  # Sortir de la boucle
        else:
            cv2.putText(frame, "NO MATCH", (20, 450), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)

        cv2.imshow("FACE RECOGNITION", frame)

    key = cv2.waitKey(1)
    if key == ord("q"):
        break

cap.release()
cv2.destroyAllWindows()
