{
  description = "QSMA Flake";

  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachSystem [ "x86_64-linux" ]
      (system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
          pkgsStatic = pkgs.pkgsStatic;
          pkgsCrossStatic = pkgs.pkgsCross.aarch64-multiplatform-musl.pkgsStatic;

          # generic build that takes a pkg set and builds the seq code
          build-qsma-seq = p:
            p.stdenv.mkDerivation rec {
              name = "qsma_sequential";
              src = ./sequential_code;
              installPhase = ''
                runHook preInstall

                mkdir --parent -- $out/
                cp --recursive -- bin $out/

                runHook postInstall
              '';
            };

          # generic build that takes a pkg set and builds the par code
          build-qsma-par = p: p.stdenv.mkDerivation rec {
            name = "qsma_parallel";
            src = ./parallel_code;
            installPhase = ''
              runHook preInstall

              mkdir --parent -- $out/bin
              cp --recursive -- FLTA $out/bin/

              runHook postInstall
            '';
          };
        in
        {
          packages = rec {
            qsma-sequential-native = build-qsma-seq pkgsStatic;
            qsma-sequential-cross = build-qsma-seq pkgsCrossStatic;
            qsma-parallel-native = build-qsma-par pkgsStatic;
            qsma-parallel-cross = build-qsma-par pkgsCrossStatic;
          };

          devShells.default = pkgs.mkShell {
            nativeBuildInputs = with pkgs; [
              qemu
            ];
          };

          checks = self.packages.${system};
        }
      );
}
