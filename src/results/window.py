import sys
import matplotlib.pyplot as plt
from PyQt6.QtWidgets import QApplication, QMainWindow, QTabWidget, QWidget, QVBoxLayout, QLabel
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure


class MplCanvas(FigureCanvas):
    def __init__(self, parent=None, width=5, height=4, dpi=100):
        fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = fig.add_subplot(111)
        super().__init__(fig)


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Results")
        self.setGeometry(100, 100, 800, 800)

        self.tabs = QTabWidget()
        self.setCentralWidget(self.tabs)

        self.tab1 = QWidget()
        self.tabs.addTab(self.tab1, "Processing")
        self.create_tab1_content()

        # Create tab 2
        self.tab2 = QWidget()
        self.tabs.addTab(self.tab2, "Communication")
        self.create_tab2_content()


    def create_tab1_content(self):
        layout = QVBoxLayout()
        self.canvas = MplCanvas(self, width=5, height=4, dpi=100)
        layout.addWidget(self.canvas)
        self.toolbar = NavigationToolbar(self.canvas, self)
        layout.addWidget(self.toolbar)
        self.tab1.setLayout(layout)
        self.plot()

    def plot(self):
        x = [0, 1, 2, 3, 4]
        y = [10, 1, 20, 3, 40]
        self.canvas.axes.plot(x, y)
        self.canvas.axes.set_title('Sample Plot')
        self.canvas.draw()

    def create_tab2_content(self):
        layout = QVBoxLayout()
        label = QLabel("This is Tab 2")
        layout.addWidget(label)
        self.tab2.setLayout(layout)


if __name__ == "__main__":
    app = QApplication(sys.argv)

    def load_stylesheet(app):
        with open("style.qss", "r") as file:
            app.setStyleSheet(file.read())

    load_stylesheet(app)

    window = MainWindow()
    window.show()
    sys.exit(app.exec())


