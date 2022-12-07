using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;

namespace Solution
{
    class Day4
    {
        private int Score(List<List<int>> b)
        {
            int sum = 0;
            foreach (var r in b) foreach (var c in r) sum += c != -1 ? c : 0;
            return sum;
        }

        private bool Win(List<List<int>> b)
        {
            for (int i = 0; i < b.Count; ++i)
            {
                bool row = true;
                for (int j = 0; j < b[i].Count; ++j)
                {
                    row = row && (b[i][j] == -1);
                }
                if (row) return true;
            }

            for (int i = 0; i < b[0].Count; ++i)
            {
                bool col = true;
                for (int j = 0; j < b.Count; ++j)
                {
                    col = col && (b[j][i] == -1);
                }
                if (col) return true;
            }
            return false;
        }

        private void Mark(List<List<int>> b, int m)
        {
            for (int i = 0; i < b.Count; ++i)
            {
                for (int j = 0; j < b[i].Count; ++j)
                {
                    if (b[i][j] == m) b[i][j] = -1;
                }
            }
        }

        private static List<List<List<int>>> ParseBoards(string[] input)
        {
            List<List<List<int>>> boards = new();
            {
                List<List<int>> b = new();
                foreach (var r in input.Skip(2).Select(s => s.Split(" ").Where(s => !string.IsNullOrWhiteSpace(s)).Select(s => int.Parse(s)).ToList()))
                {
                    if (r.Count == 0)
                    {
                        boards.Add(b);
                        b = new();
                    }
                    else
                    {
                        b.Add(r);
                    }
                }
            }

            return boards;
        }

        public int SolveOne()
        {
            string[] input = File.ReadAllLines("input/Day4.txt");
            var moves = input[0].Split(',').Select(s => int.Parse(s)).ToList();
            List<List<List<int>>> boards = ParseBoards(input);

            foreach (var m in moves)
            {
                foreach (var b in boards)
                {
                    Mark(b, m);
                    if (Win(b))
                    {
                        Console.WriteLine($"{m}, {Score(b)}, {m * Score(b)}");
                        return 0;
                    }
                }
            }

            return 0;
        }

        public int SolveTwo()
        {
            string[] input = File.ReadAllLines("input/Day4.txt");
            var moves = input[0].Split(',').Select(s => int.Parse(s)).ToList();
            List<List<List<int>>> boards = ParseBoards(input);

            List<Tuple<int, List<List<List<int>>>>> winners = new();

            foreach (var m in moves)
            {
                foreach (var b in boards)
                {
                    Mark(b, m);
                }

                List<List<List<int>>> w = boards.Where(b => Win(b)).ToList();
                if (w.Count > 0)
                {
                    boards.RemoveAll(b => Win(b));
                    winners.Add(new(m, w));
                }
            }
            winners.Reverse();
            Console.WriteLine($"{Score(winners[0].Item2[0]) * winners[0].Item1}");
            return 0;
        }

    }
    class Day5
    {
        class Line
        {
            public int x1, y1, x2, y2;

            public static Line Parse(string s)
            {
                var p = s.Split(new string[] { ",", " -> " }, StringSplitOptions.RemoveEmptyEntries).Select(p => int.Parse(p)).ToList();
                var line = new Line
                {
                    x1 = p[0],
                    y1 = p[1],
                    x2 = p[2],
                    y2 = p[3],
                };
                return line;
            }
        }

        public int SolveOne()
        {
            var input = File.ReadAllLines("input/Day5.txt").Select(s => Line.Parse(s)).Where(l => l != null).ToList();
            int[,] map = new int[1000, 1000];
            foreach (var line in input)
            {
                int wx = Math.Abs(line.x2 - line.x1);
                int dx = Math.Sign(line.x2 - line.x1);
                int wy = Math.Abs(line.y2 - line.y1);
                int dy = Math.Sign(line.y2 - line.y1);
                int w = Math.Max(wx, wy);
                if (wx != 0 && wy != 0) continue;
                for (int i = 0; i <= w; ++i)
                {
                    map[line.x1 + dx * i, line.y1 + dy * i] += 1;
                }

            }

            int result = 0;
            foreach (var r in map) result += r > 1 ? 1 : 0;

            return result;
        }


        public int SolveTwo()
        {
            var input = File.ReadAllLines("input/Day5.txt").Select(s => Line.Parse(s)).Where(l => l != null).ToList();
            int[,] map = new int[1000, 1000];
            foreach (var line in input)
            {
                int wx = Math.Abs(line.x2 - line.x1);
                int dx = Math.Sign(line.x2 - line.x1);
                int wy = Math.Abs(line.y2 - line.y1);
                int dy = Math.Sign(line.y2 - line.y1);
                int w = Math.Max(wx, wy);
                for (int i = 0; i <= w; ++i)
                {
                    map[line.x1 + dx * i, line.y1 + dy * i] += 1;
                }

            }

            int result = 0;
            foreach (var r in map) result += r > 1 ? 1 : 0;

            return result;
        }
    }
    class Day6
    {

        public long SolveOne()
        {
            var input = File.ReadAllLines("input/Day6.txt")[0].Split(',').Select(c => int.Parse(c));
            Dictionary<int, long> fishes = new() { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 }, { 5, 0 }, { 6, 0 }, { 7, 0 }, { 8, 0 }, };
            foreach (var f in input)
            {
                fishes[f] += 1;
            }
            for (int i = 0; i < 256; ++i)
            {
                long b = fishes[0];
                for (int j = 1; j <= 8; ++j)
                    fishes[j - 1] = fishes[j];
                fishes[6] += b;
                fishes[8] = b;
            }
            return fishes.Select(kv => kv.Value).Sum();
        }
    }
    class Day7
    {
        public int SolveOne()
        {
            var input = File.ReadAllLines("input/Day7.txt")[0].Split(',').Select(c => int.Parse(c)).ToList();
            input.Sort();
            int m = input.Max();
            return input.Select(i => Math.Abs(i - m)).Sum();
        }

        public int SolveTwo()
        {
            var input = File.ReadAllLines("input/Day7.txt")[0].Split(',').Select(c => int.Parse(c)).ToList();
            input.Sort();
            int m = input.Max();
            int result = int.MaxValue;
            for (int i = 0; i < m; ++i)
            {
                result = Math.Min(result, input.Select(p => Math.Abs(p - i)).Select(p => p * (p + 1) / 2).Sum());
            }
            return result;
        }
    }
    class Day8
    {
        public int SolveOne()
        {
            return File
                .ReadAllLines("Day8.txt")
                .Select(s => s.Split(" | ")[1]
                                .Split(" ")
                                .Where(s => s.Length == 2 || s.Length == 3 || s.Length == 4 || s.Length == 7)
                                .Count()).Sum();
        }

        private List<Dictionary<char, int>> coders = MutateAll("abcdefg".ToList()).Select(c => Coder(c)).ToList();

        private static Dictionary<char, int> Coder(List<char> c)
        {
            Dictionary<char, int> result = new();
            for (int i = 0; i < c.Count; ++i)
            {
                result[c[i]] = i;
            }
            return result;
        }
        private static IEnumerable<List<T>> MutateAll<T>(List<T> e)
        {
            if (e.Count == 0)
            {
                yield return new List<T>();
                yield break;
            }
            List<T> x = new(e);
            T f = x[0];
            x.RemoveAt(0);
            foreach (var p in MutateAll(x))
            {
                for (int i = 0; i <= p.Count; ++i)
                {
                    List<T> r = new(p);
                    r.Insert(i, f);
                    yield return r;
                }
            }
        }

        int? TryRead(Dictionary<char, int> c, string s)
        {
            //   000
            //  1   2
            //  1   2
            //   333
            //  4   5
            //  4   5
            //   666
            List<List<int>> digits = new()
            {
                new() { 0, 1, 2, 4, 5, 6 },
                new() { 2, 5 },
                new() { 0, 2, 3, 4, 6 },
                new() { 0, 2, 3, 5, 6 },
                new() { 1, 2, 3, 5 },
                new() { 0, 1, 3, 5, 6 },
                new() { 0, 1, 3, 4, 5, 6 },
                new() { 0, 2, 5 },
                new() { 0, 1, 2, 3, 4, 5, 6 },
                new() { 0, 1, 2, 3, 5, 6 },
            };

            var x = s.Select(d => c[d]).ToList();
            x.Sort();
            for (int i = 0; i < digits.Count; ++i)
            {
                if (digits[i].Count == x.Count)
                {
                    bool match = true;
                    for (int j = 0; j < x.Count; ++j)
                    {
                        match = match && (digits[i][j] == x[j]);
                    }
                    if (match) return i;
                }
            }
            return null;
        }

        private int Decode(List<string> x, List<string> r)
        {
            List<Dictionary<char, int>> mc = new();
            foreach (var c in coders)
            {
                HashSet<int> n = new();
                foreach (var s in x)
                {
                    var i = TryRead(c, s);
                    if (i != null) n.Add((int)i);
                }
                if (n.Count == 10) mc.Add(c);
            }

            if (mc.Count != 1) throw new Exception("cant be");
            int p = 0;
            foreach (var s in r)
            {
                p = (p * 10) + (int)TryRead(mc[0], s);
            }


            return p;
        }

        public int SolveTwo()
        {
            var input = File.ReadAllLines("input/Day8.txt").Select(s => s.Split(" | "))
                .Select(s => new Tuple<List<string>, List<string>>(s[0].Split(" ").ToList(), s[1].Split(" ").ToList())).ToList();
            return input.Select(t => Decode(t.Item1, t.Item2)).Sum();
        }
    }
    class Day9
    {
        public int SolveOne()
        {
            var input = File.ReadAllLines("input/Day9.txt").Select(s => s.Select(c => c - '0').ToArray()).ToArray();
            int[,] d = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 }, };
            int r = 0;
            for (int i = 0; i < input.Length; ++i)
            {
                for (int j = 0; j < input[i].Length; ++j)
                {
                    bool m = true;
                    for (int k = 0; k <= d.GetUpperBound(0); ++k)
                    {
                        int x = i + d[k, 0];
                        int y = j + d[k, 1];
                        if (x >= 0 && x < input.Length && y >= 0 && y < input[x].Length)
                        {
                            m = m && (input[x][y] > input[i][j]);
                        }
                    }
                    if (m)
                    {
                        r += input[i][j] + 1;
                    }
                }
            }
            return r;
        }

        private void Fill(int[][] input, int i, int j, int b)
        {
            if (input[i][j] < b) return;
            input[i][j] = b;
            int[,] d = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
            for (int k = 0; k <= d.GetUpperBound(0); ++k)
            {
                int x = i + d[k, 0];
                int y = j + d[k, 1];
                if (x >= 0 && x < input.Length && y >= 0 && y < input[x].Length)
                {
                    if (input[x][y] > b) Fill(input, x, y, b);
                }
            }
        }

        public int SolveTwo()
        {
            var input = File.ReadAllLines("input/Day9.txt").Select(s => s.Select(c => c == '9' ? -1 : int.MaxValue).ToArray()).ToArray();

            int b = 1;
            for (int i = 0; i < input.Length; ++i)
            {
                for (int j = 0; j < input[i].Length; ++j)
                {
                    Fill(input, i, j, ++b);
                }
            }

            SortedDictionary<int, int> s = new SortedDictionary<int, int>();
            for (int i = 0; i < input.Length; ++i)
            {
                for (int j = 0; j < input[i].Length; ++j)
                {
                    if (!s.ContainsKey(input[i][j])) s[input[i][j]] = 0;
                    s[input[i][j]] += 1;
                }
            }
            s.Remove(-1);
            var basins = s.OrderByDescending(x => x.Value).ToList();
            return basins.Take(3).Select(kv => kv.Value).Aggregate(1, (a, b) => a * b);
        }
    }
    class Day10
    {
        private long Score(string s)
        {
            Stack<char> p = new();
            foreach (var c in s)
            {
                switch (c)
                {
                    case '(':
                    case '[':
                    case '<':
                    case '{':
                        p.Push(c);
                        break;
                    case ')':
                        if (p.Pop() != '(') return 3;
                        break;
                    case ']':
                        if (p.Pop() != '[') return 57;
                        break;
                    case '}':
                        if (p.Pop() != '{') return 1197;
                        break;
                    case '>':
                        if (p.Pop() != '<') return 25137;
                        break;
                    default:
                        throw new Exception("wtf");
                }
            }
            long r = 0;
            while (p.Count > 0)
            {
                r *= 5;
                r += p.Pop() switch
                {
                    '(' => 1,
                    '[' => 2,
                    '{' => 3,
                    '<' => 4,
                    _ => throw new Exception("wtf"),
                };
            }
            return -r;
        }

        public long SolveOne()
        {
            var input = File.ReadAllLines("input/Day10.txt").Select(s => Score(s)).Where(s => s > 0).Sum();
            return input;
        }

        public long SolveTwo()
        {
            var input =
                File.ReadAllLines("input/Day10.txt")
                .Select(s => -Score(s)).Where(s => s > 0).OrderBy(v => v).ToList();
            return input[input.Count / 2];
        }
    }
    class Day11
    {
        public long SolveOne()
        {
            int[,] input = {
                { 2, 2, 3, 8, 5, 1, 8, 6, 1, 4 },
                { 4, 5, 5, 2, 3, 8, 8, 5, 5, 3 },
                { 2, 5, 6, 2, 1, 2, 1, 1, 4, 3 },
                { 2, 6, 6, 6, 6, 8, 5, 3, 3, 7 },
                { 7, 5, 7, 5, 5, 1, 8, 7, 8, 4 },
                { 3, 5, 7, 2, 5, 3, 4, 8, 7, 1 },
                { 8, 4, 1, 1, 7, 1, 8, 2, 8, 3 },
                { 7, 7, 4, 2, 6, 6, 8, 3, 8, 5 },
                { 1, 2, 3, 5, 1, 3, 3, 2, 3, 1 },
                { 2, 5, 4, 6, 1, 6, 5, 3, 4, 5 },
            };
            int f = 0;
            int[,] d = {
                { -1, -1 }, { 0, -1 }, { 1, -1 },
                { -1,  0 },            { 1,  0 },
                { -1,  1 }, { 0,  1 }, { 1,  1 },
                };

            void e(int i, int j)
            {
                if (i < 0 || i > 9 || j < 0 || j > 9) return;
                bool flash = input[i, j] == 9;
                ++input[i, j];
                if (flash)
                {
                    for (int k = 0; k < 8; ++k)
                        e(i + d[k, 0], j + d[k, 1]);
                }
            }
            int st = 0;
            do
            {
                ++st;
                f = 0;
                for (int j = 0; j < 10; ++j)
                    for (int k = 0; k < 10; ++k)
                        e(j, k);
                for (int j = 0; j < 10; ++j)
                    for (int k = 0; k < 10; ++k)
                        if (input[j, k] > 9)
                        {
                            input[j, k] = 0;
                            ++f;
                        }

            } while (f < 100);
            return st;
        }
    }
    class Day12
    {
        public int SolveOne()
        {
            Dictionary<string, List<string>> edges = new();

            void add(string a, string b)
            {
                if (!edges.ContainsKey(a))
                {
                    edges[a] = new();
                }
                edges[a].Add(b);
            }

            var input = File.ReadAllLines("input/Day12.txt").Select(s => s.Split("-")).Select(s => new Tuple<string, string>(s[0], s[1]));
            foreach (var e in input)
            {
                add(e.Item1, e.Item2);
                add(e.Item2, e.Item1);
            }

            Stack<string> path = new();
            bool joker = false;
            int solve(string n)
            {
                if (n == "end")
                {
                    //path.Push("end");
                    //Console.WriteLine(string.Join(",", path.Reverse().ToList()));
                    //path.Pop();
                    return 1;
                }
                bool takeJoker = false;
                if (n.ToLower() == n && path.Contains(n))
                {
                    if (!joker && n != "start")
                    {
                        takeJoker = true;
                        joker = true;
                    }
                    else
                    {
                        return 0;
                    }
                }
                path.Push(n);
                int s = 0;
                foreach (var c in edges[n])
                {
                    s += solve(c);
                }
                if (takeJoker) joker = false;
                path.Pop();
                return s;
            }

            return solve("start");
        }
    }
    class Day13
    {
        public int SolveOne()
        {
            var input = File.ReadAllLines("input/Day13.txt").Select(s => s.Split(",")).Select(s => new Tuple<int, int>(int.Parse(s[0]), int.Parse(s[1]))).ToList();
            int[,] p = new int[1311, 895];
            foreach (var d in input)
            {
                p[d.Item1, d.Item2] = 1;
            }

            int fold(int w, int x, int b)
            {
                if (x < b) return x;
                return 2 * b - x;
            }

            void foldx(int b)
            {
                int w = p.GetUpperBound(0) + 1;
                int h = p.GetUpperBound(1) + 1;
                int[,] r = new int[b + 1, h];
                for (int x = 0; x < w; ++x)
                {
                    for (int y = 0; y < h; ++y)
                    {
                        int xn = fold(w, x, b);
                        if (xn >= 0)
                            r[xn, y] = r[xn, y] | p[x, y];
                    }
                }
                p = r;
            }


            void foldy(int b)
            {
                int w = p.GetUpperBound(0) + 1;
                int h = p.GetUpperBound(1) + 1;
                int[,] r = new int[w, b + 1];
                for (int x = 0; x < w; ++x)
                {
                    for (int y = 0; y < h; ++y)
                    {
                        int yn = fold(w, y, b);
                        if (yn >= 0)
                            r[x, yn] = r[x, yn] | p[x, y];
                    }
                }
                p = r;
            }

            foldx(655);
            foldy(447);
            foldx(327);
            foldy(223);
            foldx(163);
            foldy(111);
            foldx(81);
            foldy(55);
            foldx(40);
            foldy(27);
            foldy(13);
            foldy(6);

            int w = p.GetUpperBound(0) + 1;
            int h = p.GetUpperBound(1) + 1;
            int s = 0;
            for (int y = 0; y < h; ++y)
            {
                for (int x = 0; x < w; ++x)
                {
                    Console.Write(p[x, y] != 0 ? '#' : ' ');
                }
                Console.WriteLine();
            }

            return s;
        }
    }
    class Day14
    {
        public long SolveOne()
        {
            var s = "OFSVVSFOCBNONHKFHNPK";

            var input = File.ReadAllLines("input/Day14.txt").Select(s => s.Split(" -> ")).Select(s => new Tuple<string, string>(s[0], s[1]));
            Dictionary<Tuple<char, char>, long> poly = new();
            for (int i = 0; i < s.Length - 1; ++i)
            {
                var key = new Tuple<char, char>(s[i], s[i + 1]);
                if (!poly.ContainsKey(key)) poly[key] = 0;
                poly[key] += 1;
            }
            poly[new('K', '_')] = 1;

            Dictionary<Tuple<char, char>, char> bind = new();
            foreach (var r in input)
            {
                var key = new Tuple<char, char>(r.Item1[0], r.Item1[1]);
                bind[key] = r.Item2[0];
            }
            for (int k = 0; k < 40; ++k)
            {
                Dictionary<Tuple<char, char>, long> p2 = new();
                foreach (var kv in poly)
                {
                    if (bind.ContainsKey(kv.Key))
                    {
                        var k1 = new Tuple<char, char>(kv.Key.Item1, bind[kv.Key]);
                        var k2 = new Tuple<char, char>(bind[kv.Key], kv.Key.Item2);
                        if (!p2.ContainsKey(k1)) p2[k1] = 0;
                        if (!p2.ContainsKey(k2)) p2[k2] = 0;
                        p2[k1] += kv.Value;
                        p2[k2] += kv.Value;
                    }
                    else
                    {
                        if (!p2.ContainsKey(kv.Key)) p2[kv.Key] = 0;
                        p2[kv.Key] += kv.Value;
                    }
                }
                poly = p2;
            }
            Dictionary<char, long> c = new();
            foreach (var kv in poly)
            {
                if (!c.ContainsKey(kv.Key.Item1)) c[kv.Key.Item1] = 0;
                c[kv.Key.Item1] += kv.Value;
            }
            return c.Values.Max() - c.Values.Min();
        }
    }

    class Day15
    {
        public int SolveOne()
        {
            int[,] map = new int[100, 100];
            int[,] risk = new int[100, 100];
            var input = File.ReadAllLines("input/Day15.txt");
            for (int i = 0; i < input.Length; ++i)
            {
                for (int j = 0; j < input[i].Length; ++j)
                {
                    map[i, j] = input[i][j] - '0';
                    risk[i, j] = int.MaxValue;
                }
            }
            risk[0, 0] = 0;

            int[,] d = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
            bool u = false;
            do
            {
                u = false;
                for (int i = 0; i < 100; ++i)
                {
                    for (int j = 0; j < 100; ++j)
                    {
                        for (int k = 0; k <= d.GetUpperBound(0); ++k)
                        {
                            int x = i + d[k, 0];
                            int y = j + d[k, 1];
                            if (x >= 0 && x < 100 && y >= 0 && y < 100)
                            {
                                var r = risk[x, y];
                                risk[x, y] = Math.Min(r, risk[i, j] + map[x, y]);
                                u = u || r != risk[x, y];
                            }
                        }
                    }
                }
            } while (u);

            return risk[99, 99];
        }

        public int SolveTwo()
        {
            int[,] map = new int[100, 100];
            int[,] risk = new int[500, 500];
            var input = File.ReadAllLines("input/Day15.txt");
            for (int i = 0; i < input.Length; ++i)
            {
                for (int j = 0; j < input[i].Length; ++j)
                {
                    map[i, j] = input[i][j] - '1';
                }
            }

            int w = 1 + risk.GetUpperBound(0);
            int h = 1 + risk.GetUpperBound(1);
            for (int i = 0; i < w; ++i)
            {
                for (int j = 0; j < h; ++j)
                {
                    risk[i, j] = int.MaxValue;
                }
            }

            risk[0, 0] = 0;

            int[,] d = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
            bool u;
            do
            {
                u = false;
                for (int i = 0; i < w; ++i)
                {
                    for (int j = 0; j < h; ++j)
                    {
                        for (int k = 0; k <= d.GetUpperBound(0); ++k)
                        {
                            int x = i + d[k, 0];
                            int y = j + d[k, 1];
                            if (x >= 0 && x < w && y >= 0 && y < h)
                            {
                                var r = risk[x, y];
                                var m = (map[x % 100, y % 100] + x / 100 + y / 100) % 9;
                                risk[x, y] = Math.Min(r, risk[i, j] + m + 1);
                                u = u || r != risk[x, y];
                            }
                        }
                    }
                }
            } while (u);

            return risk[w - 1, h - 1];
        }

    }

    class Day20
    {
        public int SolveOne()
        {
            var input = File.ReadAllLines("input/Day20.txt").ToList();
            Dictionary<int, bool> decoder = new();
            for (int i = 0; i < input[0].Length; ++i)
            {
                decoder[i] = input[0][i] == '#';
            }
            input = input.Skip(2).ToList();

            int w = input[0].Length;
            int h = input.Count;
            var image = new bool[w, h];
            for (int i = 0; i < w; ++i)
            {
                for (int j = 0; j < h; ++j)
                {
                    image[i, j] = input[j][i] == '#';
                }
            }

            Func<int, int, bool> raw = (i, j) =>
            {
                if (i < 0 || i >= w || j < 0 || j >= h)
                    return false;
                else
                    return image[i, j];
            };
            bool improved(int i, int j, Func<int, int, bool> m)
            {
                int[] dx = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
                int[] dy = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
                int n = 0;
                int r = 1;
                for (int k = 8; k >= 0; --k)
                {
                    var p = m(i + dx[k], j + dy[k]);
                    n += (p ? 1 : 0) * r;
                    r *= 2;
                }
                return decoder[n];
            }

            Func<int, int, bool> improve(Func<int, int, bool> g)
            {
                Dictionary<Tuple<int, int>, bool> m = new();
                Func<int, int, bool> d = (i1, i2) => g(i1, i2);
                return (i, j) =>
                {
                    Tuple<int, int> k = new(i, j);
                    if (!m.ContainsKey(k))
                    {
                        m[k] = improved(i, j, d);
                    }
                    return m[k];
                };
            }

            var res = raw;
            for (int i = 0; i < 50; ++i)
            {
                res = improve(res);
            }
            var t = improve(raw)(2, 2);

            int r = 0;
            for (int j = -h; j < h * 2; ++j)
            {
                for (int i = -w; i < w * 2; ++i)
                {
                    if (res(i, j))
                    {
                        r += 1;
                        //Console.Write('#');
                    }
                    else
                    {
                        //Console.Write('.');
                    }
                }
                // Console.WriteLine();
            }
            return r;
        }
    }

    class Day19
    {
        List<Tuple<int, int, int>> Rotate(Tuple<int, int, int> p)
        {
            int x = p.Item1, y = p.Item2, z = p.Item3;
            return new()
            {
                new(x, y, z),
                new(x, -y, -z),
                new(x, z, -y),
                new(x, -z, y),
                new(-x, z, y),
                new(-x, -z, -y),
                new(-x, -y, z),
                new(-x, y, -z),
                new(y, z, x),
                new(y, -z, -x),
                new(y, -x, z),
                new(y, x, -z),
                new(-y, x, z),
                new(-y, -x, -z),
                new(-y, -z, x),
                new(-y, z, -x),
                new(z, x, y),
                new(z, -x, -y),
                new(z, -y, x),
                new(z, y, -x),
                new(-z, x, -y),
                new(-z, -x, y),
                new(-z, y, x),
                new(-z, -y, -x),
            };
        }

        List<HashSet<Tuple<int, int, int>>> Rotate(HashSet<Tuple<int, int, int>> p)
        {
            List<HashSet<Tuple<int, int, int>>> result = new();
            for (int i = 0; i < 24; ++i)
            {
                result.Add(new());
            }
            foreach (var d in p)
            {
                var r = Rotate(d);
                for (int i = 0; i < 24; ++i)
                {
                    result[i].Add(r[i]);
                }
            }
            return result;
        }

        HashSet<Tuple<int, int, int>> Pivot(Tuple<int, int, int> o, HashSet<Tuple<int, int, int>> s)
        {
            return s.Select(s => new Tuple<int, int, int>(
                s.Item1 - o.Item1,
                s.Item2 - o.Item2,
                s.Item3 - o.Item3)).ToHashSet();
        }

        Tuple<int, Tuple<int, int, int>, HashSet<Tuple<int, int, int>>> Match(HashSet<Tuple<int, int, int>> a, HashSet<Tuple<int, int, int>> b)
        {
            foreach (var pa in a)
            {
                List<HashSet<Tuple<int, int, int>>> rot = Rotate(b);
                for (int i = 0; i < rot.Count; i++)
                {
                    HashSet<Tuple<int, int, int>> r = rot[i];
                    foreach (var pb in r)
                    {
                        var o = new Tuple<int, int, int>(pb.Item1 - pa.Item1, pb.Item2 - pa.Item2, pb.Item3 - pa.Item3);
                        var bset = Pivot(o, r);
                        var iset = a.Intersect(bset).ToHashSet();
                        if (iset.Count >= 12) return new(i, o, bset);
                    }
                }
            }
            return null;
        }
        private List<Scanner> TryMatch(List<Scanner> a, List<Scanner> b)
        {
            for (int i = 0; i < a.Count; ++i)
            {
                for (int j = 0; j < b.Count; ++j)
                {
                    var m = Match(a[i].Probes, b[j].Probes);
                    if (m != null)
                    {
                        List<Scanner> r = new();
                        r.AddRange(a);
                        r.AddRange(b.Select(
                            s => 
                            new Scanner(Pivot(m.Item2, new() { Rotate(s.Location)[m.Item1] }).First(),
                            Pivot(m.Item2, Rotate(s.Probes)[m.Item1]))));
                        return r;
                    }
                }
            }
            return null;
        }

        class Scanner
        {
            public Tuple<int, int, int> Location = new(0, 0, 0);
            public HashSet<Tuple<int, int, int>> Probes = new();

            public Scanner() { }

            public Scanner(Tuple<int, int, int> location, HashSet<Tuple<int, int, int>> probes)
            {
                Location = location ?? throw new ArgumentNullException(nameof(location));
                Probes = probes ?? throw new ArgumentNullException(nameof(probes));
            }
        }

        public int SolveOne()
        {
            List<List<Scanner>> beacons = new();
            Scanner scanner = null;
            foreach (var s in File.ReadAllLines("input/Day19.txt"))
            {
                if (s.StartsWith("---"))
                {
                    scanner = new();
                    beacons.Add(new() { scanner });
                }
                else if (s.Length > 0)
                {
                    var c = s.Split(',').Select(s => int.Parse(s)).ToArray();
                    scanner.Probes.Add(new(c[0], c[1], c[2]));
                }
            }

            while (beacons.Count > 1)
            {
                var a = beacons[0];
                beacons.RemoveAt(0);
                bool match = false;
                for (int i = 0; i < beacons.Count; ++i)
                {
                    var c = TryMatch(a, beacons[i]);
                    if (c != null)
                    {
                        beacons.RemoveAt(i);
                        beacons.Add(c);
                        Console.WriteLine($"{i} - {beacons.Count}");
                        match = true;
                        break;
                    }
                }
                if (!match) throw new Exception("wtf?");
            }

            HashSet<Tuple<int, int, int>> r = new();
            foreach (var b in beacons[0])
            {
                r.UnionWith(b.Probes);
            }

            int md = 0;
            for (int i = 0; i < beacons[0].Count; ++i)
            {
                for (int j = 0; j < beacons[0].Count; ++j)
                {
                    var a = beacons[0][i].Location;
                    var b = beacons[0][j].Location;
                    int d = Math.Abs(a.Item1 - b.Item1) + Math.Abs(a.Item2 - b.Item2) + Math.Abs(a.Item3 - b.Item3);
                    if (d > md) md = d;
                }
            }
            Console.WriteLine($"Max distanve = {md}");

            return r.Count;
        }
    }

    class Day21
    {
        public int SolveOne()
        {
            int n = 0;
            int p1 = 8 - 1, p2 = 7 - 1;
            int d = 1;
            int s1 = 0, s2 = 0;

            void roll (ref int p, ref int d)
            {
                p = (p + d) % 10;
                d += 1;
            }

            while (true)
            {
                for (int i = 0; i < 3; ++i)
                {
                    n += 1;
                    roll(ref p1, ref d);
                }
                s1 += (p1 + 1);
                if (s1 >= 1000) return s2 * n;
                for (int i = 0; i < 3; ++i)
                {
                    n += 1;
                    roll(ref p2, ref d);
                }
                s2 += (p2 + 1);
                if (s2 >= 1000) return s1 * n;
            }
        }    
    }

    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine(new Day20().SolveOne());
        }
    }
}