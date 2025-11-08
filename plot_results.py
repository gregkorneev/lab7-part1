import os, csv, argparse
from collections import defaultdict
import matplotlib
matplotlib.use("Agg")     # безопасно в WSL/без GUI
import matplotlib.pyplot as plt

CSV_DIR   = "csv"
PLOTS_DIR = "plots"

def ensure_dirs():
    os.makedirs(PLOTS_DIR, exist_ok=True)

def sniff_delimiter(path):
    """Определяем разделитель по первой непустой строке."""
    with open(path, "r", encoding="utf-8-sig", newline="") as f:
        for line in f:
            s = line.strip()
            if s:
                # если оба есть — берём тот, которого больше
                sc, cm = s.count(';'), s.count(',')
                if sc == 0 and cm == 0: return ';'
                return ';' if sc >= cm else ','
    return ';'

def read_rows(path, expected_cols):
    """Читает CSV с autodetect разделителя и BOM. Возвращает список словарей либо None (если файла нет/колонки не найдены)."""
    if not os.path.exists(path):
        print(f"[SKIP] Нет файла: {path}")
        return None

    delim = sniff_delimiter(path)
    rows = []
    with open(path, "r", encoding="utf-8-sig", newline="") as f:
        reader = csv.reader(f, delimiter=delim)
        header = next(reader, [])
        header = [h.strip() for h in header]

        # если весь заголовок склеен в один столбец из-за неверного делимитера — пробуем расколоть вручную
        if len(header) == 1 and (',' in header[0] or ';' in header[0]):
            header = [h.strip() for h in header[0].replace(';', delim).split(delim)]

        # проверим наличие нужных колонок
        missing = [c for c in expected_cols if c not in header]
        if missing:
            print(f"[SKIP] {path}: нет колонок {missing} (есть: {header})")
            return None

        idx = {name: header.index(name) for name in expected_cols}
        for row in reader:
            if not row:
                continue
            # аналогично защитимся от «склейки» строк
            if len(row) == 1 and (',' in row[0] or ';' in row[0]):
                row = [c.strip() for c in row[0].replace(';', delim).split(delim)]
            # пропустим кривые строки
            if len(row) <= max(idx.values()):
                continue
            rows.append({name: row[idx[name]].strip() for name in expected_cols})
    return rows

def plot_lines_generic(path, title, xlab, ylab, outname):
    rows = read_rows(path, ["n", "method", "time_ms"])
    if rows is None: return
    data = defaultdict(lambda: {"n": [], "t": []})
    for r in rows:
        try:
            data[r["method"]]["n"].append(int(float(r["n"])))
            data[r["method"]]["t"].append(float(r["time_ms"]))
        except ValueError:
            continue
    if not data:
        print(f"[SKIP] {path}: нет валидных данных")
        return
    plt.figure()
    for method, series in sorted(data.items()):
        pairs = sorted(zip(series["n"], series["t"]))
        xs = [p[0] for p in pairs]
        ys = [p[1] for p in pairs]
        plt.plot(xs, ys, marker="o", label=method)
    plt.yscale("log")
    plt.xlabel(xlab)
    plt.ylabel(ylab)
    plt.title(title)
    plt.legend()
    out = os.path.join(PLOTS_DIR, outname)
    plt.savefig(out, dpi=150, bbox_inches="tight"); plt.close()
    print(f"[OK] {out}")

def plot_sorting_sizes():
    plot_lines_generic(
        os.path.join(CSV_DIR, "sorting_sizes.csv"),
        "Сортировки: время vs n",
        "Размер массива (n)",
        "Время, мс (лог шкала)",
        "sorting_sizes.png",
    )

def plot_search_sizes():
    plot_lines_generic(
        os.path.join(CSV_DIR, "search_sizes.csv"),
        "Поиск: последовательный vs бинарный",
        "Размер массива (n)",
        "Время, мс (лог шкала)",
        "search_sizes.png",
    )

def plot_closest_sizes():
    plot_lines_generic(
        os.path.join(CSV_DIR, "closest_sizes.csv"),
        "Поиск ближайших точек",
        "Количество точек (n)",
        "Время, мс (лог шкала)",
        "closest_sizes.png",
    )

def plot_sorting_cases():
    path = os.path.join(CSV_DIR, "sorting_cases.csv")
    rows = read_rows(path, ["Algorithm", "Case", "Time_ms"])
    if rows is None: return

    order_alg  = ["SelectionSort", "BubbleSort", "MergeSort"]
    order_case = ["Лучший", "Средний", "Худший"]
    by_case = {c: [0.0, 0.0, 0.0] for c in order_case}
    for r in rows:
        alg, case = r["Algorithm"], r["Case"]
        if alg in order_alg and case in order_case:
            try:
                t = float(r["Time_ms"])
            except ValueError:
                continue
            by_case[case][order_alg.index(alg)] = t

    import numpy as np
    x = np.arange(len(order_alg)); w = 0.25
    plt.figure()
    for i, c in enumerate(order_case):
        plt.bar(x + (i-1)*w, by_case[c], width=w, label=c)
    plt.xticks(x, order_alg); plt.ylabel("Время, мс")
    plt.title("Сортировки: лучший/средний/худший")
    plt.legend()
    out = os.path.join(PLOTS_DIR, "sorting_cases.png")
    plt.savefig(out, dpi=150, bbox_inches="tight"); plt.close()
    print(f"[OK] {out}")

def main():
    ensure_dirs()
    jobs = {
        "sorting": plot_sorting_sizes,
        "search":  plot_search_sizes,
        "closest": plot_closest_sizes,
        "cases":   plot_sorting_cases,
    }
    ap = argparse.ArgumentParser()
    ap.add_argument("--only", nargs="*", choices=list(jobs.keys()))
    args = ap.parse_args()
    if args.only:
        for k in args.only: jobs[k]()
    else:
        for fn in jobs.values(): fn()

if __name__ == "__main__":
    main()
