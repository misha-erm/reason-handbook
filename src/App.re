type route =
  | FormRoute
  | GraphQlRoute
  | CounterRoute
  | NotFoundRoute;

type state = {route};

type action =
  | ChangeRoute(route);

let reducer = (action, _state) =>
  switch (action) {
  | ChangeRoute(route) => ReasonReact.Update({route: route})
  };

let matchUrlToComponent = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | []
  | ["counter"] => CounterRoute
  | ["form"] => FormRoute
  | ["graphQl"] => GraphQlRoute
  | _ => NotFoundRoute
  };

let changeRoute = (route, _event, self) =>
  self.ReasonReact.send(ChangeRoute(route));

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {
    route: ReasonReact.Router.dangerouslyGetInitialUrl()->matchUrlToComponent,
  },
  reducer,
  didMount: self => {
    let watchId =
      ReasonReact.Router.watchUrl(url =>
        url->matchUrlToComponent->ChangeRoute->(self.send)
      );
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watchId));
  },
  render: self =>
    <>
      <h2> {ReasonReact.string("ReasonReact Handbook")} </h2>
      <ul className="nav">
        <li onClick={_event => ReasonReact.Router.push("/counter")}>
          {ReasonReact.string("Counter")}
        </li>
        <li onClick={_event => ReasonReact.Router.push("/form")}>
          {ReasonReact.string("Form")}
        </li>
        <li onClick={_event => ReasonReact.Router.push("/graphQl")}>
          {ReasonReact.string("GraphQl")}
        </li>
      </ul>
      <div>
        {
          switch (self.state.route) {
          | FormRoute => <Form />
          | GraphQlRoute => <GraphQl />
          | CounterRoute => <Counter />
          | NotFoundRoute => ReasonReact.string("NOT FOUND")
          }
        }
      </div>
    </>,
};