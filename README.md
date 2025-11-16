# 🌍 Global Map Navigator  
### A 3D & 2D GIS Navigation Engine — C++ Crow Backend + CesiumJS Frontend

This project implements a **full-scale global 3D map navigation system** using:

- **C++ Backend (Crow + Asio)**
- **CesiumJS 3D Globe Renderer**
- **OpenStreetMap / Nominatim APIs**
- **Routing Algorithms (Dijkstra / A*)**
- **Day–Night shading simulation**
- **City search, reverse geocoding & route visualization**

It combines **computer graphics**, **data structures & algorithms**, **networking**, and **GIS computation** into one real-world engineering project.

---

## 🚀 Features

### 🌐 Global 3D Globe (CesiumJS)
- Full WGS84 ellipsoid
- High-resolution satellite tiles
- Smooth camera fly-to animations
- Day/night solar terminator effect
- Realistic lighting, terrain & atmosphere

### 🔎 Smart Location Search
- Search any city worldwide  
- Backend fetches geocoding data  
- Camera flies to the coordinates  

### 🛣 Shortest Path Routing (Graph Algorithms)
- Uses Dijkstra / A*  
- Roads modeled as a weighted graph  
- Route visualized as a 3D polyline  
- Extendable to real-time traffic  

### 💻 C++ Crow Backend
- REST API endpoints  
- Async networking (Asio)  
- JSON parsing (nlohmann/json)  
- Clean modular architecture  

### 📦 Fully Open Source
Designed to be modifiable, extendable, and educational.

---

## 📂 Project Structure

```
global-map-navigator/
│
├── backend/
│   ├── src/
│   │   ├── main.cpp
│   │   ├── router.cpp
│   │   ├── geocode.cpp
│   │   └── route_engine.cpp
│   ├── include/
│   ├── CMakeLists.txt
│   ├── README.md
│
├── frontend/
│   ├── index.html
│   ├── app.js
│   ├── styles.css
│   ├── assets/
│
├── docs/
│   ├── THEORY_FULL_REPORT.pdf
│   ├── THEORY_ONLY_FULL_REPORT.pdf
│   ├── EXTENDED_LONG_REPORT.pdf
│   └── screenshots/
│
├── .gitignore
└── README.md
```

---

## ⚙️ Installation & Setup

### 1️⃣ Clone the repository

```bash
git clone https://github.com/keshavchawla2015-svg/global-map-navigator.git
cd global-map-navigator
```

---

## 🖥️ Backend (C++17)

### Build Requirements
- C++17
- CMake
- Crow library
- Asio
- nlohmann/json

### Build Steps

```bash
cd backend
mkdir build && cd build
cmake ..
make
./navigator_api --port 8080
```

---

## 🌐 Frontend (CesiumJS)

Simply open:

```
frontend/index.html
```

Or start a local HTTP server:

```bash
cd frontend
python3 -m http.server 5500
```

Open:

```
http://localhost:5500
```

---

## 🔄 API Endpoints

### `/search?city={name}`
Returns:
- latitude  
- longitude  
- display name  
- bounding box  

### `/route?from=A&to=B`
Returns:
- polyline route coordinates  
- path distance  
- metadata  

---

## 🧠 Algorithms Used

- **Dijkstra** — Weighted graph shortest path  
- **A\*** — Heuristic accelerated pathfinding  
- **Haversine Formula** — Earth-distance calculation  
- **Web Mercator Projection**  
- **Lat/Lon → Cartesian3 conversion**  
- **Lambertian lighting for day/night shading**  

---

## 📘 Documentation

All theory reports are in:

```
docs/
```

Includes:
- **Full Theory Report**
- **Extended Long PDF**
- **Clean Theory-Only Report**
- **Screenshots** (manually added)

---

## 📌 Future Scope

- Real-time traffic integration  
- ML-based dynamic route selection  
- Weather overlays  
- Cesium 3D Tiles integration  
- Offline tile caching  
- VR & AR exploration modes  

---

## 📝 License

This project is fully open-source under the **MIT License**.

---

## ⭐ Support

Leave a star ⭐ on the repo if you like the work!

