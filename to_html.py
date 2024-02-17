from datetime import datetime as dt
import mpld3
import os

def figs_to_html (figs):
    htmls = []
    for fig in figs:
        htmls.append(mpld3.fig_to_html(fig))
    return htmls

def to_html (filename, figs):
    htmls = figs_to_html(figs)
    
    combined_html = f"""
    <!DOCTYPE html>
    <html>
    <head>
    <title>Mis Figuras</title>
    <script src="https://d3js.org/d3.v5.min.js"></script>
    <script src="https://mpld3.github.io/js/mpld3.v0.5.5.js"></script>
    <style>
        body {{
            text-align: center;
            margin: 20px; /* Ajusta el margen según tus preferencias */
        }}
        .figure-container {{
            display: inline-block;
            margin: 10px; 
            border: 1px solid #ddd; 
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
            background-color: #f9f9f9; 
            padding: 10px; 
        }}
    </style>
    </head>
    <body>
    """

    # Agregar cada figura al HTML combinado
    for html_fig in htmls:
        combined_html += f'<div class="figure-container">{html_fig}</div>\n'

    combined_html += """
    </body>
    </html>
    """
    os.makedirs('results', exist_ok=True)
    with open(filename, 'w') as f:
        f.write(combined_html)

    return combined_html

