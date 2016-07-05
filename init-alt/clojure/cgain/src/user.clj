(ns user
  (:require [clojure.tools.namespace.repl :refer [refresh]]
            [clojure.tools.trace :as tr]
            [clojure.pprint :refer [pprint print-table]]
            #_[clojure.repl :refer :all]
            [clojure.java.io :as io]
            [cgain.util :as util :refer [ifprob]])
  (:use [cgain.core]
        [incanter core stats charts]
        #_[clojure.core.matrix]))

#_(set-current-implementation :vectorz)
