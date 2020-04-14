#include <iostream>
#include <map>
#include <vector>
#include <set>

const int ALF_SIZE = 256;

class Point {
 public:
  Point() : x(0), y(0) {}

  Point(int x1, int y1) : x(x1), y(y1) {}

  int operator<(const Point &other) const {
    if (other.x == x) return y < other.y;
    return x < other.x;
  }

  int x, y;
};

class AhoCorasick {
 public:
  AhoCorasick() {
    vertexes_.emplace_back();
  }

  void AddVector(const std::vector<size_t> &vector, Point point) {
    int current_vertex_num = 0;
    for (size_t i = 0; i < vector.size(); ++i) {
      size_t new_symbol = vector[i];
      if (vertexes_[current_vertex_num].next[new_symbol] == -1) {
        vertexes_.emplace_back(false, current_vertex_num, new_symbol, -1);
        vertexes_[current_vertex_num].next[new_symbol] = GetSize_() - 1;
      }
      current_vertex_num = vertexes_[current_vertex_num].next[new_symbol];
    }

    vertexes_[current_vertex_num].leaf = true;
    vertexes_[current_vertex_num].points.push_back(point);
  }

  void CountLinks() {
    for (int i = 0; i < GetSize_(); ++i) {
      int prev = GetLink_(i);
      if (vertexes_[prev].leaf) {
        vertexes_[i].lazy_link = prev;
      } else {
        vertexes_[i].lazy_link = vertexes_[prev].lazy_link;
      }
    }
  }

  std::vector<Point> SearchVector(const std::vector<size_t> &searchable) {
    int current_pos = 0;
    for (int i = 0; i < searchable.size(); ++i) {
      int symbol = searchable[i];
      current_pos = Go_(current_pos, symbol);
    }
    std::vector<Point> result;
    while (current_pos != -1) {
      for (Point p : vertexes_[current_pos].points) {
        result.push_back(p);
      }
      current_pos = vertexes_[current_pos].lazy_link;
    }
    return result;
  }

 private:
  int GetLink_(int vertex_num) {
    if (vertexes_[vertex_num].link == -1) {
      if (vertex_num == 0 || vertexes_[vertex_num].parent == 0) {
        vertexes_[vertex_num].link = 0;
      } else {
        vertexes_[vertex_num].link =
            Go_(GetLink_(vertexes_[vertex_num].parent), vertexes_[vertex_num].parent_symbol);
      }
    }
    return vertexes_[vertex_num].link;
  }

  int Go_(int vertex_num, int symbol) {
    if (vertexes_[vertex_num].go[symbol] == -1) {
      if (vertexes_[vertex_num].next[symbol] != -1) {
        vertexes_[vertex_num].go[symbol] = vertexes_[vertex_num].next[symbol];
      } else {
        vertexes_[vertex_num].go[symbol] = vertex_num == 0 ? 0 : Go_(GetLink_(vertex_num), symbol);
      }
    }
    return vertexes_[vertex_num].go[symbol];
  }

  class Vertex_ {
   public:
    Vertex_() : link(-1), parent(-1), lazy_link(-1), next(ALF_SIZE, -1), go(ALF_SIZE, -1) {
    }

    Vertex_(bool nleaf, int nparent, int nparent_symbol, int nlink)
        : leaf(nleaf),
          parent(nparent),
          parent_symbol(nparent_symbol),
          link(nlink),
          lazy_link(-1),
          next(ALF_SIZE, -1),
          go(ALF_SIZE, -1) {
    }

    std::vector<int> next;
    bool leaf;
    int parent;
    int parent_symbol;
    int link;
    int lazy_link;
    std::vector<int> go;
    std::vector<Point> points;
  };

  size_t GetSize_() {
    return vertexes_.size();
  }

  std::vector<Vertex_> vertexes_;
};

class Picture {
 public:
  Picture() {
    int x_size, y_size;
    std::cin >> x_size >> y_size;
    for (int i = 0; i < x_size; ++i) {
      picture.emplace_back();
      for (int j = 0; j < y_size; ++j) {
        int next;
        std::cin >> next;
        picture.back().push_back(next);
      }
    }
  }

  size_t GetX() {
    return picture.size();
  }

  size_t GetY() {
    return picture[0].size();
  }

  void AddLinesToAhoCorasick(size_t width, AhoCorasick &aho_corasick) {
    for (int i = 0; i < GetX(); ++i) {
      for (int j = 0; j + width <= GetY(); ++j) {
        std::vector<size_t> to_add(width);
        for (int k = 0; k < width; ++k) {
          to_add[k] = picture[i][j + k];
        }
        aho_corasick.AddVector(to_add, Point(i, j));
      }
    }
  }

  std::vector<Point> SearchLineInAhoCorasick(size_t line_num, AhoCorasick &aho_corasick) {
    return aho_corasick.SearchVector(picture[line_num]);
  }

  void Rotate() {
    std::vector<std::vector<size_t>> rotated;
    for (int i = 0; i < GetY(); ++i) {
      rotated.emplace_back();
      for (int j = 0; j < GetX(); ++j) {
        rotated.back().push_back(picture[GetX() - j - 1][i]);
      }
    }
    picture = std::move(rotated);
  }

  std::vector<std::vector<size_t>> picture;
};
int main() {
  Picture picture1 = Picture();
  Picture picture2 = Picture();

  for (int i = 0; i < 4; ++i) {
    picture2.Rotate();

    AhoCorasick aho_corasick = AhoCorasick();

    picture1.AddLinesToAhoCorasick(picture2.GetY(), aho_corasick);

    std::set<Point> points_set;

    auto first_line = picture2.SearchLineInAhoCorasick(0, aho_corasick);

    for (Point p: first_line) {
      points_set.insert(p);
    }

    for (int i = 1; i < picture2.GetX(); ++i) {
      std::set<Point> new_set;

      auto next_line = picture2.SearchLineInAhoCorasick(i, aho_corasick);
      for (Point p: next_line) {
        if (points_set.find(Point(p.x - 1, p.y)) != points_set.end()) {
          new_set.insert(p);
        }
      }
      points_set = std::move(new_set);
    }
    if (!points_set.empty()) {
      Point p = *points_set.begin();
      std::cout << p.x + 1 - picture2.GetX() << " " << p.y;
      return 0;
    }
  }
  std::cout << "Second picture is not in first picture";
}
