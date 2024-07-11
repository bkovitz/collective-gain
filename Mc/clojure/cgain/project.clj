(defproject cgain "0.1.0-SNAPSHOT"
  :description "Collective Gain: an evolutionary simulation"
  :url "http://example.com/FIXME"
  :license {:name "Eclipse Public License"
            :url "http://www.eclipse.org/legal/epl-v10.html"}
  :dependencies [[org.clojure/clojure "1.8.0"]
                 [net.mikera/core.matrix "0.52.2"]
                 [net.mikera/vectorz-clj "0.44.1"]
                 [org.clojure/math.numeric-tower "0.0.4"]
                 [org.clojure/tools.macro "0.1.2"]
                 [org.clojure/tools.namespace "0.2.11"]
                 [org.clojure/tools.trace "0.7.9"]
                 [incanter "1.5.7"]
                 [criterium "0.4.4"]]
  :main ^:skip-aot cgain.core
  :repl-options {:init-ns user}
  :target-path "target/%s"
  :profiles {:uberjar {:aot :all}})
