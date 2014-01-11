#!/usr/bin/python3
import argparse
import subprocess
from xml.dom import minidom
import sys
import os

current_dir = os.path.dirname(os.path.realpath(__file__))

# Works only on Fedora (tools are called lupdate-qt5 and lrelease-qt5)
lupdate = "lupdate-qt5"
lrelease = "lrelease-qt5"

def getText(element):
    if element.firstChild is None:
        return None
    text = element.firstChild
    if text.nodeType != text.TEXT_NODE:
        return None
    return text.wholeText

def copy(element, doc):
    element_copy = doc.createElement(element.tagName)
    for attribute in element.attributes.keys():
        element_copy.setAttribute(attribute, element.getAttribute(attribute))
    # Copy text
    text = getText(element)
    if text is None:
        return element_copy
    text_copy = doc.createTextNode(text)
    element_copy.appendChild(text_copy)
    return element_copy

def do_push():
    # Execute lupdate
    subprocess.call([lupdate, "src", "-ts", "%s/src/translations/friends-tmp.ts" % current_dir])
    
    # Open the translation file and do a fake translation
    doc = minidom.parse("%s/src/translations/friends-tmp.ts" % current_dir)
    ts = doc.documentElement
        
    final_doc = minidom.Document()
    imp =  minidom.getDOMImplementation('')
    dt = imp.createDocumentType('TS', '', '')
    final_doc.appendChild(dt)
    
    # Copy TS
    final_ts = copy(ts, doc)
    final_ts.setAttribute("language", "en")
    final_doc.appendChild(final_ts)
    
    # Copy contexts
    contexts = ts.getElementsByTagName("context")
    for context in contexts:
        final_context = copy(context, doc)
        final_ts.appendChild(final_context)
        
        # Copy context name
        names = context.getElementsByTagName("name")
        for name in names:
            final_context.appendChild(copy(name, doc))
        
        # Copy messages
        messages = context.getElementsByTagName("message")
        for message in messages:
            final_message = copy(message, doc)
            final_context.appendChild(final_message)
            
            # Copy location / source / extra comment
            locations = message.getElementsByTagName("location")
            for location in locations:
                final_message.appendChild(copy(location, doc))
            sources = message.getElementsByTagName("source")
            for source in sources:
                sourceText = getText(source)
                final_message.appendChild(copy(source, doc))
            extra_comments = message.getElementsByTagName("extracomment")
            for extra_comment in extra_comments:
                final_message.appendChild(copy(extra_comment, doc))
            
            translation = doc.createElement("translation")
            final_message.appendChild(translation)
            if message.hasAttribute("numerus"):
                # We need to add <numerusform>text</numerusform> twice
                for i in range(0, 2):
                    numerus_form = doc.createElement("numerusform")
                    text_copy = doc.createTextNode(sourceText)
                    numerus_form.appendChild(text_copy)
                    translation.appendChild(numerus_form)
            else:
                text_copy = doc.createTextNode(sourceText)
                translation.appendChild(text_copy)
    
    f = open("%s/src/translations/friends.ts" % current_dir, 'w')
    final_doc.writexml(f, encoding="utf-8")
    f.close()
    
    subprocess.call(["rm", "%s/src/translations/friends-tmp.ts" % current_dir])
    subprocess.call(["tx", "push", "-s"])

def do_pull(perc):
    subprocess.call(["rm *.ts"], cwd="%s/src/translations" % current_dir, shell=True)
    subprocess.call(["rm *.qm"], cwd="%s/src/translations" % current_dir, shell=True)
    subprocess.call(["tx", "pull", "-a", "--minimum-perc=%s" % perc])
    subprocess.call(["%s -idbased *.ts" % lrelease], cwd="%s/src/translations" % current_dir, shell=True)
    subprocess.call(["rm *.ts"], cwd="%s/src/translations" % current_dir, shell=True)
    subprocess.call([lupdate, "src", "-ts", "%s/src/translations/friends.ts" % current_dir])
    subprocess.call(["%s -idbased friends.ts -qm friends-engineering-english.qm" % lrelease], cwd="%s/src/translations" % current_dir, shell=True)
    subprocess.call(["rm *.ts"], cwd="%s/src/translations" % current_dir, shell=True)

parser = argparse.ArgumentParser(description='Friends translation management script')
parser.add_argument('--push', action='store_true', help='Push source translation file')
parser.add_argument('--pull', action='store_true', help='Pull translation file')
parser.add_argument('percentage', metavar='percentage', type=int, nargs='?',
                    help='Percentage of translation required')

args = parser.parse_args()
push = args.push
pull = args.pull
percentage = args.percentage

if push and pull or (not push and not pull):
    parser.print_help()
    sys.exit(1)

if percentage is not None and not pull:
    parser.print_help()
    sys.exit(1)
   
if percentage is not None:
    if percentage < 0 or percentage > 100:
        parser.print_help()
        sys.exit(1)
    else:
        percentage = 100
if push:
    do_push()
    sys.exit(0)

if pull:
    do_pull(percentage)
    sys.exit(0)