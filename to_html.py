from datetime import datetime as dt
import mpld3
import os

def figs_to_html (cat_figs, scr_figs, chk_figs):
    cat_htmls = []
    scr_htmls = []
    chk_htmls = []
    for fig in cat_figs:
        cat_htmls.append(mpld3.fig_to_html(fig))
    
    for fig in scr_figs:
        scr_htmls.append(mpld3.fig_to_html(fig))
    
    for fig in chk_figs:
        chk_htmls.append(mpld3.fig_to_html(fig))
    
    return cat_htmls, scr_htmls, chk_htmls

def to_html (filename, cat_figs, scr_figs, chk_figs):
    cat_htmls, scr_htmls, chk_htmls = figs_to_html(cat_figs, scr_figs, chk_figs)
    
    combined_html = f"""
    <!DOCTYPE html>
    <html>
    <head>
    <title>Figuras</title>
    <script src="https://d3js.org/d3.v5.min.js"></script>
    <script src="https://mpld3.github.io/js/mpld3.v0.5.5.js"></script>
    <style>
        body {{
            text-align: center;
            margin: 30px; /* Ajusta el margen según tus preferencias */
        }}
        .figure-container {{
            display: inline-block;
            margin: 10px; 
            border: 1px solid #ddd; 
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
            background-color: #f9f9f9; 
            padding: 10px; 
        }}
        .titles {{
            text-align: center;
            font-family: 'Arial', sans-serif; /* Cambia 'Arial' por la fuente que prefieras */
            color: #333030; /* Cambia el color del texto según tus preferencias */
            text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.2); /* Añade sombra al texto */
        }}
    </style>
    </head>
    <body>

    <h1 class="titles">{filename}</h1>
    """

    # Agregar cada figura al HTML combinado
    combined_html += f'<h2 class="titles">Category</h2>\n'
    for html_fig in cat_htmls:
        combined_html += f'<div class="figure-container">{html_fig}</div>\n'

    combined_html += f'<h2 class="titles">Screening</h2>\n'
    for html_fig in scr_htmls:
        combined_html += f'<div class="figure-container">{html_fig}</div>\n'

    combined_html += f'<h2 class="titles">Checkers</h2>\n'
    for html_fig in chk_htmls:
        combined_html += f'<div class="figure-container">{html_fig}</div>\n'

    combined_html += """
    </body>
    </html>
    """
    os.makedirs('results', exist_ok=True)
    with open("results/" + filename + ".html", 'w') as f:
        f.write(combined_html)

    return combined_html

