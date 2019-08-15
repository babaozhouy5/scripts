#!/bin/bash

# A very simple terminal dictionary

if [ $# -lt 1 ]; then
    echo "usage: $0 query"
    exit 0
fi

init()
{
    ret=$(which jq > /dev/null 2>&1 ; echo $?)
    if [ $ret -ne 0 ]; then
        echo "no jq found, installing..."
        mkdir -p ~/.local/bin
        wget -q https://github.com/stedolan/jq/releases/download/jq-1.6/jq-linux64 -O ~/.local/bin/jq && chmod u+x ~/.local/bin/jq
    fi
}

detect_language()
{
    query="$*"
    query_encode=`echo -ne $query | jq -sRr @uri`
    language=`curl -s http://10.141.176.105:8080/langdetect?query=$query_encode | jq '.lan' -r`
    echo $language
}

translate()
{
    lang="$1"
    if [ -z "$lang" ]; then
        echo "Not support language!"
        exit 1
    elif [ "$lang" = "zh" ]; then
        result=`curl -s -d "content=$QUERY" "http://translator.speech.sogou.com/index.mt?type=1&domain=0&from=zh&to=en"`
    else
        result=`curl -s -d "content=$QUERY" "http://translator.speech.sogou.com/index.mt?type=1&domain=0&from=$lang&to=zh"`
    fi
    echo $result
}

QUERY="$*"
init
translate $(detect_language $QUERY)
