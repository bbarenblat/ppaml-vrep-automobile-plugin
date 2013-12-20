;; .dir-locals.el -- directory-local variables for the car plugin
;; Copyright (C) 2013  Galois, Inc.
;;
;; This library is free software: you can redistribute it and/or modify it under
;; the terms of the GNU General Public License as published by the Free Software
;; Foundation, either version 3 of the License, or (at your option) any later
;; version.
;;
;; This library is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
;; FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
;; details.
;;
;; You should have received a copy of the GNU General Public License along with
;; this library.  If not, see <http://www.gnu.org/licenses/>.
;;
;; To contact Galois, complete the Web form at <http://corp.galois.com/contact/>
;; or write to Galois, Inc., 421 Southwest 6th Avenue, Suite 300, Portland,
;; Oregon, 97204-1622.


((nil . ((fill-column . 80)))
 (autoconf-mode . ((indent-tabs-mode . nil)))
 (makefile-automake-mode . ((indent-tabs-mode . t)))
 (c++-mode . ((c-file-style . "bsd")
	      (c-basic-offset . 4)
              (indent-tabs-mode . nil)))
 ;; This is not a polyglot library, so open .h files in C++ mode.
 (c-mode . ((mode . c++)
            (c-file-style . "bsd")
            (c-basic-offset . 4)
            (indent-tabs-mode . nil)))
 (text-mode . ((indent-tabs-mode . nil))))
