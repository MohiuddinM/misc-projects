#!/var/virtualenv/bin/python

import dns.resolver
import socket
import sys

FromDomain = ""
FromAddr = ""

errors = ["421", "450", "451", "452", "500", "501", "502", "503", "504", "521", "530", "550", "551", "552", "553", "554"]

def check(domain, ToAddr):
    try:
        MxServer = str(dns.resolver.query(domain, 'MX')[0].exchange).rstrip('.')

        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
        s.connect((MxServer, 25))
    
        resp = str(s.recv(1024)).encode()
    
        s.send("HELO {0}\r\n".format(FromDomain))
        resp += str(s.recv(1024))

        s.send("MAIL FROM: <{0}>\r\n".format(FromAddr))
        resp += str(s.recv(1024))

        s.send("RCPT TO: <{0}>\r\n".format(ToAddr))
        resp += str(s.recv(1024))

        s.close()
        for error in errors:
            if error in resp:
                status = "failed"
                break;
            else:
                status = "successful"

        return status, resp.encode()

    except Exception, e:
        return "failed", "Connection Problem"


def main():

    # I created this script to help a client with some seo related tasks. This script could be used to verify large number of emails.
    # verify-email.org is a paid service which does the same task
    # Only use this service on a well known webhost server, otherwise popular email services e.g. gmail, yahoo, outlook will never accept connections.
    
    #Usage: python2 email_verifier.py [From_Email_Address] [Email_List_To_Verify] [Verified_Emails_Log]
    #E.g. : python2 email_verifier.py email@example.com /tmp/emails.txt /tmp/verified_emails.txt
    
    global FromDomain, FromAddr

    FromAddr = str(sys.argv[1])
    FromDomain = FromAddr.split("@")[1]
    emailsfile = open(str(sys.argv[2]), 'rb')
    logfile = open(str(sys.argv[3]), 'wb')

    for emails in emailsfile.readlines():
        email = emails.strip()
        status, message = check(email.split("@")[1], email)
        print "{0} - {1} \n {2}".format(email, status, message)
        logfile.write("{0} - {1}\n".format(email, status))
        logfile.flush()

if __name__ == "__main__":
    main()
