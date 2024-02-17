import resend
from os import environ


def send_to_email(operation, message):
    resend_key = "RESEND_KEY"
    email = "EMAIL"

    if not resend_key in environ or not email in environ:
        print("Necesary environment variables to send to email not found.")
        return
    

    resend.api_key = resend_key
    r = resend.Emails.send({
        "from": "onboarding@resend.dev",
        "to": f"d{email}",
        "subject": f"{operation} - Codee",
        "html": f"{message}"
    })
