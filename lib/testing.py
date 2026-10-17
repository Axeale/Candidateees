from pathlib import Path


def crear_archivos_por_subcarpeta():
	carpeta_actual = Path(__file__).resolve().parent

	for subcarpeta in carpeta_actual.iterdir():
		if subcarpeta.is_dir():
			(subcarpeta / f"{subcarpeta.name}.h").touch(exist_ok=True)
			(subcarpeta / f"{subcarpeta.name}.cpp").touch(exist_ok=True)


if __name__ == "__main__":
	crear_archivos_por_subcarpeta()
