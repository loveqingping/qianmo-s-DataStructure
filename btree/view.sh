#!/usr/bin/env bash
#  Filename: view.sh
#   Created: 2016-12-02 15:17:00
#      Desc: TODO (some description)
#    Author: dusk(qianmo du), dusk@ifeng.com
#   Company: Ifeng
./btree_test $1  > example.dot && dot -Tpdf example.dot -o example.pdf 
