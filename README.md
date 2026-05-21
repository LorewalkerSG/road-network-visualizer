# visualize_roadnetwork

This project converts OpenStreetMap data into a road network graph and visualizes it interactively in the browser.

The overall workflow has 3 stages:

1. Download map data from OSM
2. Process the raw map with two C++ programs
3. Visualize the processed network with HTML + Python

## Project Structure

- [osm2graph.cpp](/mnt/visualize_roadnetwork/osm2graph.cpp): converts an OSM/XML map into a raw graph
- [raw_graph_process.cpp](/mnt/visualize_roadnetwork/raw_graph_process.cpp): clips the graph by boundary and extracts the largest connected component
- [road_network_frontend.html](/mnt/visualize_roadnetwork/road_network_frontend.html): frontend visualization page
- [road_network_frontend_server.py](/mnt/visualize_roadnetwork/road_network_frontend_server.py): local static file server
- `*.co`, `*.gr`: processed road network data files

## Workflow Overview

### 1. Download OSM Map Data

First, prepare an OSM map file. The converter reads an XML-style OSM file.

Common sources:

- Geofabrik regional downloads
- OpenStreetMap export
- Any tool that can export OSM XML

It is recommended to place the downloaded map file in the project directory, for example:

```text
map.osm
```

## 2. Use `osm2graph.cpp` to Generate the Raw Graph

This program:

- parses `node` entries in OSM
- parses `way` entries in OSM
- connects adjacent road points into edges
- computes edge weights from geographic coordinates
- writes a raw `.gr` file and a raw `.co` file

The usage comment in the source code is:

```bash
./osm2graph map.osm raw.gr raw.co
```

### Compile

```bash
g++ -O2 -std=c++11 osm2graph.cpp -o osm2graph
```

### Run

```bash
./osm2graph map.osm raw.gr raw.co
```

### Output Format

`raw.gr`:

- first line: `N M`
- each following line: `u v w`
- vertex ids here are internal program indices, starting from `0`
- `w` is the edge weight computed from geographic distance

`raw.co`:

- first line: `N`
- each following line: `lat lon`
- coordinates are multiplied by `1e6` and stored as integers
- there is no explicit vertex id in this file; row order is the vertex index

## 3. Use `raw_graph_process.cpp` to Process the Raw Graph

This program performs two additional steps:

1. clip the graph using a given geographic boundary
2. extract the largest connected component from the clipped graph

It then generates the final `.co` and `.gr` files used by the frontend.

The usage comment in the source code is:

```bash
./raw_graph_process raw.gr out.gr raw.co out.co bound.txt
```

### Compile

```bash
g++ -O2 -std=c++11 raw_graph_process.cpp -o raw_graph_process
```

### Run

```bash
./raw_graph_process raw.gr final.gr raw.co final.co bound.txt
```

### Input Arguments

Argument order:

1. input raw graph `raw.gr`
2. output graph `final.gr`
3. input raw coordinates `raw.co`
4. output coordinates `final.co`
5. boundary file `bound.txt`

### `bound.txt` Format

The program reads 4 numbers from the boundary file in this order:

```text
max_lat min_lon max_lon min_lat
```

This matches the internal reading order:

```cpp
latb[1] lonb[0] lonb[1] latb[0]
```

### Output Format

Processed `final.co`:

- first line: `N`
- each following line: `id lon lat`
- vertex ids start from `1`
- longitude and latitude are still stored as integers multiplied by `1e6`

Processed `final.gr`:

- first line: `N M`
- each following line: `a b w`
- vertex ids start from `1`
- each undirected edge is stored twice: `a b w` and `b a w`

This is the format expected by the frontend.

## Frontend Visualization

[road_network_frontend.html](/mnt/visualize_roadnetwork/road_network_frontend.html) is the main visualization page. It:

- loads `.co` and `.gr`
- renders the road network with WebGL
- supports drag-to-pan and wheel-to-zoom
- supports selection mode
- supports vertex hover
- supports shortest-path computation after selecting two vertices
- highlights the shortest path in red

Note: the shortest path algorithm is not executed by the Python server. It runs in the browser frontend using JavaScript.

## How to Start the Frontend

[road_network_frontend_server.py](/mnt/visualize_roadnetwork/road_network_frontend_server.py) is only a local static file server used to serve the HTML page to the browser.

### Start

```bash
python road_network_frontend_server.py
```

By default, it serves:

```text
http://127.0.0.1:8000/road_network_frontend.html
```

### Custom Host / Port

```bash
python road_network_frontend_server.py --host 127.0.0.1 --port 8000
```

## Full Example

```bash
g++ -O2 -std=c++11 osm2graph.cpp -o osm2graph
g++ -O2 -std=c++11 raw_graph_process.cpp -o raw_graph_process

./osm2graph map.osm raw.gr raw.co
./raw_graph_process raw.gr NY-d.gr raw.co NY-d.co bound.txt

python road_network_frontend_server.py
```

Then open:

```text
http://127.0.0.1:8000/road_network_frontend.html
```

After that, select `NY-d.co` and `NY-d.gr` in the left panel to load the sample network.

## Included Example Files

The project already includes one sample dataset:

- [NY-d.co](/mnt/visualize_roadnetwork/NY-d.co)
- [NY-d.gr](/mnt/visualize_roadnetwork/NY-d.gr)

You can directly start the frontend and load them for testing.

## Notes

- `osm2graph.cpp` currently parses OSM XML text directly and does not depend on a dedicated OSM parsing library
- `raw_graph_process.cpp` outputs the largest connected component after clipping, which is usually more suitable for visualization and path queries
- the frontend currently focuses on interactive browsing and shortest-path visualization, and can be extended later with more graph interaction features
