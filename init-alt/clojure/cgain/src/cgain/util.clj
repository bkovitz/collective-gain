(ns cgain.util
  (:refer-clojure :exclude [rand rand-int])
  (:require [clojure.tools.trace :refer :all]
            [clojure.pprint :refer [pprint]]
            [clojure.math.numeric-tower :as math]
            [clojure.tools.trace :refer :all]
            [clojure.java.io :as io]
            [clojure.edn :as edn]))

;; debugging aids

(defmacro dde
  "\"Display for debugging.\" Surround an expression that you want to print
  with dd, and it will print it and pass the value through. Precede with
  other expressions and it will print those, too, preceded by the expression.
  A string literal will print as itself on a line by itself, to help you
  find it in a stream of output."
  [expr & more]
  (cond
    (and (nil? expr) (nil? more))
      `(println "dde: no args")
    (nil? more)
      `(let [result# ~expr]
         (print "=> ")
         (prn result#)
         result#)
    :else
      (if (string? expr)
          `(do
             (prn ~expr)
             (dde ~@more))
          `(let [v# ~expr
                v# (if (coll? v#) (doall v#) v#)]
             (prn '~expr v#)
             (dde ~@more)))))

(defn workspace? [x]
  (try
    (and (map? x) (= :workspace (:type x)))
    (catch ClassCastException e
      false)))

(defn dd-str [x]
  (with-out-str
    (if (workspace? x)
        (pr "(a workspace)")
        (print x))))

(defn dd1- [expr]
  (cond
    (= \newline expr)
      `(println)
    (string? expr)
      `(prn ~expr)
    :else
      `(println '~expr "=>" (dd-str ~expr))))

(defn- dd- [exprs]
  (cond
    (empty? exprs)
      nil
    (empty? (rest exprs))
      `(do
         ~(dd1- (first exprs))
         ~(first exprs))
    :else
      `(do
         ~(dd1- (first exprs))
         ~(dd- (rest exprs)))))

(defmacro dd
  "\"Display for debugging.\" Prints each expr and what it evaluates to.
  Returns the value of the last expr. Does not double-print string literals."
  [& exprs]
  (dd- exprs))

;; Random-number generation

(def ^:dynamic *rng-seed* (. System nanoTime))
  ;; The seed that *rng* started with; not necessarily its current seed.

(defn make-rng [seed]
  (java.util.Random. seed))

(def ^:dynamic *rng* (make-rng *rng-seed*))

(defmacro with-rng-seed [n & body]
  `(binding [*rng-seed* ~n, *rng* (make-rng ~n)]
     ~@body))

(defn rand
 ([]
  (.nextDouble *rng*))
 ([n]
  (* n (rand))))

(defn rand-int [n]
  (int (rand n)))

; By Christophe Grand (modified by Ben Kovitz to force v to be a vector)
; https://groups.google.com/d/msg/clojure/Kj0b_YhXcos/UahpU7m3iJcJ
(defn lazy-shuffle [v]
  (let [v (vec v)]
    (lazy-seq
      (if (seq v)
          (let [idx (rand-int (count v))]
            (cons (nth v idx)
              (lazy-shuffle (pop (assoc v idx (peek v))))))))))

(defmacro ifprob [p yes no]
  `(if (<= (rand) ~p) ~yes ~no))

;; Miscellaneous

(defn clamp [lb ub x]
  (cond
    (< x lb) lb
    (> x ub) ub
    :else x))
